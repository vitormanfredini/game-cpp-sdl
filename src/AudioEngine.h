#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "BinaryResourceLoader.h"
#include <unordered_map>

class AudioEngine {
public:
    AudioEngine(){}

    ~AudioEngine(){
        cleanup();
        SDL_CloseAudio();
    }

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
        desiredSpec.samples = 4096;
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

        auto it = stringIdToSound.find(filename);
        if (it != stringIdToSound.end()) {
            return stringIdToIntId[filename];
        }

        BinaryResource binaryResource = BinaryResourceLoader::getBinaryResource(filename.c_str());

        Uint8* buffer;
        Uint32 length;
        SDL_AudioSpec spec;
        SDL_RWops* rw = SDL_RWFromConstMem(binaryResource.data, binaryResource.length);
        if (SDL_LoadWAV_RW(rw, 1, &spec, &buffer, &length) == nullptr) {
            std::cerr << "AudioEngine loadSound: Failed to load WAV (" << filename << "): " << SDL_GetError() << std::endl;
            return -1;
        }

        Sound* newSound = new Sound(reinterpret_cast<Sint16*>(buffer), length / 2);

        stringIdToSound[filename] = newSound;
        intIdToSound[nextId] = newSound;
        stringIdToIntId[filename] = nextId;
        nextId++;

        return nextId - 1;
    }

    void playSound(int id, int position = 0){
        if(id <= 0){
            return;
        }
        SDL_LockAudio();
        soundsPlaying.emplace_back(intIdToSound[id], position);
        SDL_UnlockAudio();
    }

    static void audioCallbackWrapper(void* userdata, Uint8* stream, int len) {
        static_cast<AudioEngine*>(userdata)->audioCallback(reinterpret_cast<Sint16*>(stream), len / 2);
    }

    void audioCallback(Sint16* stream, int len) {
        SDL_memset(stream, 0, len * sizeof(Sint16));

        for (auto& soundPlaying : soundsPlaying) {
            if (soundPlaying.isFinished()) continue;

            Uint32 remainingSamples = soundPlaying.sound->length - soundPlaying.position;
            Uint32 mixSamples = (remainingSamples < len) ? remainingSamples : len;

            for (Uint32 i = 0; i < mixSamples; ++i) {
                stream[i] = stream[i] + soundPlaying.sound->buffer[soundPlaying.position + i];
            }

            soundPlaying.position += mixSamples;
        }

        hardLimiter(stream, len);

        removeFinishedSounds();
    }

private:
    std::vector<SoundPlaying> soundsPlaying;

    std::unordered_map<std::string, Sound*> stringIdToSound;
    std::unordered_map<int, Sound*> intIdToSound;
    std::unordered_map<std::string, int> stringIdToIntId;
    
    int nextId = 1;

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

    void hardLimiter(Sint16* stream, int len) {
        for (Uint32 i = 0; i < len; ++i) {
            Sint32 sample = stream[i];

            if (sample > 32767){
                sample = 32767;
            } else if (sample < -32768){
                sample = -32768;
            }

            stream[i] = sample;
        }
    }

    void cleanup() {
        SDL_LockAudio();
        
        for (const auto& pair : intIdToSound) {
            SDL_FreeWAV(reinterpret_cast<Uint8*>(pair.second->buffer));
            delete pair.second;
        }

        soundsPlaying.clear();
        stringIdToSound.clear();
        intIdToSound.clear();
        stringIdToIntId.clear();
        nextId = 1;

        SDL_UnlockAudio();
    }

};
