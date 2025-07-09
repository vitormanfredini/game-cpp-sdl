#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "BinaryResourceLoader.h"

class AudioEngine {
public:
    AudioEngine(){}

    struct Sound {
        Sint16* buffer;
        Uint32 length;
        Uint32 position;

        Sound(Sint16* buf, Uint32 len) : buffer(buf), length(len), position(0) {}
        bool isFinished() const { return position >= length; }
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

    void loadSound(const std::string& filename) {

        BinaryResource binaryResource = BinaryResourceLoader::getBinaryResource(filename.c_str());

        Uint8* buffer;
        Uint32 length;
        SDL_AudioSpec spec;
        SDL_RWops* rw = SDL_RWFromConstMem(binaryResource.data, binaryResource.length);
        if (SDL_LoadWAV_RW(rw, 1, &spec, &buffer, &length) == nullptr) {
            std::cerr << "AudioEngine loadSound: Failed to load WAV (" << filename << "): " << SDL_GetError() << std::endl;
            return;
        }

        SDL_LockAudio();
        activeSounds.emplace_back(reinterpret_cast<Sint16*>(buffer), length / 2);
        SDL_UnlockAudio();

    }

    static void audioCallbackWrapper(void* userdata, Uint8* stream, int len) {
        static_cast<AudioEngine*>(userdata)->audioCallback(reinterpret_cast<Sint16*>(stream), len / 2);
    }

    void audioCallback(Sint16* stream, int len) {
        SDL_memset(stream, 0, len * sizeof(Sint16));

        for (auto& sound : activeSounds) {
            if (sound.isFinished()) continue;

            Uint32 remainingSamples = sound.length - sound.position;
            Uint32 mixSamples = (remainingSamples < len) ? remainingSamples : len;

            for (Uint32 i = 0; i < mixSamples; ++i) {
                Sint32 sample = stream[i] + sound.buffer[sound.position + i];

                if (sample > 32767){
                    sample = 32767;
                } else if (sample < -32768){
                    sample = -32768;
                }

                stream[i] = static_cast<Sint16>(sample);
            }

            sound.position += mixSamples;
        }

        auto it = activeSounds.begin();
        while (it != activeSounds.end()) {
            if (it->isFinished()) {
                SDL_FreeWAV(reinterpret_cast<Uint8*>(it->buffer));
                it = activeSounds.erase(it);
            } else {
                ++it;
            }
        }
    }

    void cleanup() {
        for (auto& sound : activeSounds) {
            SDL_FreeWAV(reinterpret_cast<Uint8*>(sound.buffer));
        }
        SDL_CloseAudio();
    }

private:
    std::vector<Sound> activeSounds;
};
