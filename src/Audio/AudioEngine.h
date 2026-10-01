#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "BinaryResourceLoader.h"
#include <unordered_map>
#include "CacheManager.h"
#include "BeatManager.h"
#include "DynamicSong.h"

#define BUFFER_SIZE 512

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
        float gain;

        SoundPlaying(Sound* sound, Uint32 offset, Uint32 position, float gain)
            :   sound(sound),
                offset(offset),
                position(position),
                gain(gain) {}

        bool isFinished() const {
            return position >= sound->length;
        }
    };

    struct SoundScheduledToUpdate {
        int id;
        int update;
        float gain;

        SoundScheduledToUpdate(
            int id,
            int update,
            float gain
        ): 
            id(id),
            update(update),
            gain(gain) {
                //
            }
    };

    AudioEngine(){
        bufferTimeMs = (static_cast<double>(BUFFER_SIZE) / 44100.0) * 1000.0;
        onUpdateFinished(0);
        maxCorrection = BUFFER_SIZE / 2;
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

    void setDynamicSong(DynamicSong* newDynamicSong){
        dynamicSong = newDynamicSong;
    }

    void onAdvanceLevel(int level){
        dynamicSong->triggerSectionChange(level);
    }

    void onUpdateFinished(int update){
        lastUpdateFinished = update;
        timeLastUpdateFinished = SDL_GetPerformanceCounter();
    }

    void startBeat(){
        onUpdateFinished(0);

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

        double timeMsSinceLastUpdateFinished = (double) ((SDL_GetPerformanceCounter() - timeLastUpdateFinished)*1000.0 / (double) SDL_GetPerformanceFrequency());
        int samplesUntilNextBeat = beatManagerUpdates.samplesUntilBeat(lastUpdateFinished);

        double timeMsUntilNextBeat = (static_cast<double>(samplesUntilNextBeat) / 44100.0) * 1000.0;
        double timeMsLastUpdateToNextBeat = timeMsSinceLastUpdateFinished + timeMsUntilNextBeat;

        double minimumSafeTimeMsLastUpdateToNextBeat = 20.0 + bufferTimeMs;
        double maximumAllowedTimeMsLastUpdateToNextBeat = 20.0 + bufferTimeMs + 20.0;

        int smallCorrectionInSamples = 0;
        if(timeMsLastUpdateToNextBeat < minimumSafeTimeMsLastUpdateToNextBeat){
            double fullCorrectionInMs = minimumSafeTimeMsLastUpdateToNextBeat - timeMsLastUpdateToNextBeat;
            smallCorrectionInSamples = static_cast<int>((fullCorrectionInMs * 0.05 * 44100.0) / 1000.0);
        }else if(timeMsLastUpdateToNextBeat > maximumAllowedTimeMsLastUpdateToNextBeat){
            double fullCorrectionInMs = maximumAllowedTimeMsLastUpdateToNextBeat - timeMsLastUpdateToNextBeat;
            smallCorrectionInSamples = static_cast<int>((fullCorrectionInMs * 0.05 * 44100.0) / 1000.0);
        }

        if(smallCorrectionInSamples > maxCorrection){
            smallCorrectionInSamples = maxCorrection;
        }else if(smallCorrectionInSamples < -maxCorrection){
            smallCorrectionInSamples = -maxCorrection;
        }

        std::vector<BeatManager::BeatUpdateAndOffset> updateBeatsOffsets = beatManagerUpdates.updateAndGetBeatsUpdatesAndOffsets(length, smallCorrectionInSamples);
        for(BeatManager::BeatUpdateAndOffset beatOffset : updateBeatsOffsets){
            auto it = soundsScheduledForUpdates.begin();
            while (it != soundsScheduledForUpdates.end()) {
                if(it->update == beatOffset.beat){
                    playSound(it->id, beatOffset.offset, 0, it->gain);
                    it = soundsScheduledForUpdates.erase(it);
                } else if(beatOffset.beat > it->update) {
                    std::cerr << "beat is " << beatOffset.beat << ". scheduled sound is for beat: " << it->update << ". removing from schedule." << std::endl;
                    it = soundsScheduledForUpdates.erase(it);
                }else{
                    ++it;
                }
            }
        }

        std::vector<BeatManager::BeatUpdateAndOffset> musicBeatsOffsets = beatManagerMusic.updateAndGetBeatsUpdatesAndOffsets(length);
        for(BeatManager::BeatUpdateAndOffset beatOffset : musicBeatsOffsets){
            for(int soundId : dynamicSong->getCurrentSectionCurrentBeatAudios()){
                playSound(soundId, beatOffset.offset, 0, musicGain);
            }
        }

        for (int i = 0; i < length; ++i) {
            auxBuffer[i] = 0;
        }

        for (auto& soundPlaying : soundsPlaying) {
            Uint32 remainingSamples = soundPlaying.sound->length - soundPlaying.position;
            Uint32 mixSamples = (static_cast<int>(remainingSamples) < length) ? remainingSamples : length;

            bool shouldApplyoffset = soundPlaying.position == 0 && soundPlaying.offset > 0;
            
            Uint32 initialIndex = 0;
            if(shouldApplyoffset){
                initialIndex = soundPlaying.offset;
                Uint32 lengthAfterOffset = length - soundPlaying.offset;
                mixSamples = (remainingSamples < lengthAfterOffset) ? remainingSamples : lengthAfterOffset;
            }

            for (Uint32 i = 0; i < mixSamples; ++i) {
                float sampleWithGainApplied = soundPlaying.sound->buffer[soundPlaying.position + i] * soundPlaying.gain;
                auxBuffer[initialIndex + i] = auxBuffer[initialIndex + i] + sampleWithGainApplied;
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

    void scheduleSoundToUpdate(int id, int update, float gain){
        soundsScheduledForUpdates.emplace_back(id, update, gain);
    }

private:

    int auxBuffer[BUFFER_SIZE];
    double bufferTimeMs;

    BeatManager beatManagerMusic { 117600 }; // number of samples per wav/beat
    BeatManager beatManagerUpdates { 735 }; // number of samples per game engine update

    std::vector<SoundScheduledToUpdate> soundsScheduledForUpdates = {};

    std::vector<SoundPlaying> soundsPlaying;

    DynamicSong* dynamicSong;
    float musicGain = 1.0f;

    int lastUpdateFinished;
    Uint64 timeLastUpdateFinished;

    int maxCorrection;

    void playSound(int id, int offset, int position, float gain){
        if(id <= 0){
            return;
        }
        SDL_LockAudio();
        soundsPlaying.emplace_back(soundsCache.get(id), offset, position, gain);
        SDL_UnlockAudio();
    }

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
        soundsScheduledForUpdates.clear();
        lastUpdateFinished = 0;
        
        SDL_UnlockAudio();
    }

};
