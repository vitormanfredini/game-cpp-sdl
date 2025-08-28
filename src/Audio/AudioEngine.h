#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "BinaryResourceLoader.h"
#include <unordered_map>
#include "CacheManager.h"
#include "BeatManager.h"
#include "MissionSong.h"

#define BUFFER_SIZE 4096

class AudioEngine {
public:

    struct Sound {
        Sint16* buffer;
        Uint32 length;
        
        Sound(Sint16* buf, Uint32 len)
            :   buffer(buf),
                length(len) {}
    };

    struct SoundPlaying {
        Sound* sound;
        Uint32 offset;
        Uint32 position;

        SoundPlaying(Sound* sound, Uint32 offset, Uint32 position)
            :   sound(sound),
                offset(offset),
                position(position) {}

        bool isFinished() const {
            return position >= sound->length;
        }
    };

    AudioEngine(){
        strongTick = loadSound("audio/click_strong.wav");
    }

    ~AudioEngine(){
        cleanup();
        SDL_CloseAudio();
    }

    bool init() {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_AudioSpec desiredSpec;
        SDL_zero(desiredSpec);
        desiredSpec.freq = 44100;
        desiredSpec.format = AUDIO_S16SYS;
        desiredSpec.channels = 1;
        desiredSpec.samples = BUFFER_SIZE;
        desiredSpec.callback = &AudioEngine::audioCallbackWrapper;
        desiredSpec.userdata = this;

        if (SDL_OpenAudio(&desiredSpec, NULL) < 0) {
            std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        SDL_PauseAudio(0);
        return true;
    }

    int loadSound(const std::string& filename) {
        return soundsCache.load(filename);
    }

    void setMissionSong(MissionSong* newMissingSong){
        missionSong = newMissingSong;
    }

    void playSound(int id, int offset = 0, int position = 0){
        if(id <= 0){
            return;
        }
        SDL_LockAudio();
        soundsPlaying.emplace_back(soundsCache.get(id), offset, position);
        SDL_UnlockAudio();
    }

    void onAdvanceLevel(int level){
        missionSong->changeLevel(level);
    }

    void startBeat(){
        beatManagerMusic.reset();
        beatManagerMusic.play();

        beatManagerUpdates.reset();
        beatManagerUpdates.play();
    }

    static void audioCallbackWrapper(void* userdata, Uint8* stream, int len) {
        static_cast<AudioEngine*>(userdata)->audioCallback(reinterpret_cast<Sint16*>(stream), len / 2);
    }

    void audioCallback(Sint16* stream, int length) {
        if(length != BUFFER_SIZE){
            std::cerr << "AudioEngine audioCallback(): len != BUFFER_SIZE" << std::endl;
            return;
        }

        for (int i = 0; i < length; ++i) {
            auxBuffer[i] = 0;
        }

        std::vector<int> musicBeatsOffsets = beatManagerMusic.updateAndGetBeatsOffsets(length);
        for(int offset : musicBeatsOffsets){
            // std::cout << "offset: " << offset << std::endl;
            for(int soundId : missionSong->getLevelLoopSounds()){
                playSound(soundId, offset);
            }
        }

        std::vector<int> updateBeatsOffsets = beatManagerUpdates.updateAndGetBeatsOffsets(length);
        for(int offset : updateBeatsOffsets){
            // playSound(strongTick, offset);
        }

        for (auto& soundPlaying : soundsPlaying) {
            Uint32 remainingSamples = soundPlaying.sound->length - soundPlaying.position;
            Uint32 mixSamples = (remainingSamples < length) ? remainingSamples : length;

            bool shouldApplyoffset = soundPlaying.position == 0 && soundPlaying.offset > 0;
            
            Uint32 initialIndex = 0;
            if(shouldApplyoffset){
                initialIndex = soundPlaying.offset;
                Uint32 lengthAfterOffset = length - soundPlaying.offset;
                mixSamples = (remainingSamples < lengthAfterOffset) ? remainingSamples : lengthAfterOffset;
            }

            for (Uint32 i = 0; i < mixSamples; ++i) {
                auxBuffer[initialIndex + i] = auxBuffer[initialIndex + i] + soundPlaying.sound->buffer[soundPlaying.position + i];
            }

            soundPlaying.position += mixSamples;
        }

        hardLimiter(auxBuffer, length);

        SDL_memset(stream, 0, length * sizeof(Sint16));
        for (int i = 0; i < length; ++i) {
            stream[i] = auxBuffer[i];
        }

        removeFinishedSounds();

    }

private:

    int auxBuffer[BUFFER_SIZE];

    BeatManager beatManagerMusic { 25442 * 4 };

    BeatManager beatManagerUpdates { 11025 };

    std::vector<SoundPlaying> soundsPlaying;

    MissionSong* missionSong;

    int strongTick = 0;

    CacheManager<Sound*> soundsCache {
        [](std::string& filename) -> Sound* {
            BinaryResource binaryResource = BinaryResourceLoader::getBinaryResource(filename.c_str());

            Uint8* buffer;
            Uint32 length;
            SDL_AudioSpec spec;
            SDL_RWops* rw = SDL_RWFromConstMem(binaryResource.data, binaryResource.length);
            if (SDL_LoadWAV_RW(rw, 1, &spec, &buffer, &length) == nullptr) {
                std::cerr << "AudioEngine loadSound: Failed to load WAV (" << filename << "): " << SDL_GetError() << std::endl;
                return nullptr;
            }

            return new Sound(reinterpret_cast<Sint16*>(buffer), length / 2);
        },
        [](Sound* sound){
            SDL_FreeWAV(reinterpret_cast<Uint8*>(sound->buffer));
            delete sound;
        },
    };

    void removeFinishedSounds(){
        auto it = soundsPlaying.begin();
        while (it != soundsPlaying.end()) {
            if (it->isFinished()) {
                it = soundsPlaying.erase(it);
            } else {
                ++it;
            }
        }
    }

    void hardLimiter(int stream[], int len) {
        for (int i = 0; i < len; ++i) {
            if (stream[i] > 32767){
                stream[i] = 32767;
            } else if (stream[i] < -32768){
                stream[i] = -32768;
            }
        }
    }

    void cleanup() {
        SDL_LockAudio();

        soundsCache.clear();
        soundsPlaying.clear();
        
        SDL_UnlockAudio();
    }

};
