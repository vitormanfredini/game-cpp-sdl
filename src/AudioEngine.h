#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "BinaryResourceLoader.h"
#include <unordered_map>
#include "CacheManager.h"

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
        Uint32 position;

        SoundPlaying(Sound* sound, int position)
            :   sound(sound),
                position(position) {}

        bool isFinished() const {
            return position >= sound->length;
        }
    };

    AudioEngine(){
        weakTick = loadSound("audio/song1/click_weak.wav");
        strongTick = loadSound("audio/song1/click_strong.wav");
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

    void playSound(int id, int position = 0){
        if(id <= 0){
            return;
        }
        SDL_LockAudio();
        soundsPlaying.emplace_back(soundsCache.get(id), position);
        SDL_UnlockAudio();
    }

    static void audioCallbackWrapper(void* userdata, Uint8* stream, int len) {
        static_cast<AudioEngine*>(userdata)->audioCallback(reinterpret_cast<Sint16*>(stream), len / 2);
    }

    void audioCallback(Sint16* stream, int len) {
        if(len != BUFFER_SIZE){
            std::cerr << "AudioEngine audioCallback(): len != BUFFER_SIZE" << std::endl;
            return;
        }

        for (int i = 0; i < len; ++i) {
            auxBuffer[i] = 0;
        }

        // beatSamplesCount += len;
        // if(beatSamplesCount >= samplesPerBeat){
        //     if(measureBeatsCount == 0){
        //         playSound(strongTick);
        //     }else{
        //         playSound(weakTick);
        //     }
        //     beatSamplesCount -= samplesPerBeat;
        //     int offsetToBeat = len - beatSamplesCount;
        //     measureBeatsCount++;
        //     if(measureBeatsCount == 4){
        //         measureBeatsCount = 0;
        //         measuresCount++;
        //     }
        // }
        

        for (auto& soundPlaying : soundsPlaying) {
            Uint32 remainingSamples = soundPlaying.sound->length - soundPlaying.position;
            Uint32 mixSamples = (remainingSamples < len) ? remainingSamples : len;

            for (Uint32 i = 0; i < mixSamples; ++i) {
                auxBuffer[i] = auxBuffer[i] + soundPlaying.sound->buffer[soundPlaying.position + i];
            }

            soundPlaying.position += mixSamples;
        }

        hardLimiter(auxBuffer, len);

        SDL_memset(stream, 0, len * sizeof(Sint16));
        for (int i = 0; i < len; ++i) {
            stream[i] = auxBuffer[i];
        }

        removeFinishedSounds();

        
    }

private:

    int auxBuffer[BUFFER_SIZE];

    // const int samplesPerBeat = 24054; // 24054 samples = about 0.54 seconds
    // int beatSamplesCount = 0;
    // int measureBeatsCount = 0;
    // int measuresCount = 0;

    int weakTick = 0;
    int strongTick = 0;

    std::vector<SoundPlaying> soundsPlaying;

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
