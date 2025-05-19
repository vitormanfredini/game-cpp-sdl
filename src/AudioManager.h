#pragma once

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "BinaryResourceLoader.h"
#include "SDLUtils.h"
#include "GameObject/GameObject.h"
#include "RenderProps.h"
#include "Font/FontManager.h"
#include <string>
#include <sstream>

class AudioManager {

public:

    AudioManager() { };

    ~AudioManager(){
        clearCache();
    }

    Mix_Chunk* loadAudio(const std::string& filename){

        Mix_Chunk* cachedTexture = loadFromCache(filename);
        if(cachedTexture != nullptr){
            return cachedTexture;
        }

        Mix_Chunk* texture = BinaryResourceLoader::toAudioChunk(filename.c_str());
        if (texture) {
            cache[filename] = texture;
        }
        return texture;

    }

    void clearCache() {
        for (auto& pair : cache) {
            Mix_FreeChunk(pair.second);
        }
        cache.clear();
    }

    void playAudio(Mix_Chunk* sound){
        playAudio(sound, 0);
    }

    void playAudio(Mix_Chunk* sound, int offsetInUpdates){
        Mix_Chunk* soundToPlay = sound;
        if(offsetInUpdates > 0){
            soundToPlay = sideloadChunkWithOffset(sound, offsetInUpdates);
        }

        int channel = Mix_PlayChannel(-1, soundToPlay, 0); // 0 = play once (not looped)
        if (channel == -1) {
            std::cerr << "Failed to play audio: " << Mix_GetError() << std::endl;
        }
    }

private:
    std::unordered_map<std::string, Mix_Chunk*> cache;

    Mix_Chunk* sideloadChunkWithOffset(Mix_Chunk* originalChunk, int offsetInUpdates) {

        int samplesPerUpdate = 44100 / 60;  // Assuming 60 updates per second
        int samplesOffset = offsetInUpdates * samplesPerUpdate;

        // Convert to byte offset
        int bytesPerSample = 4;  // 16-bit mono
        int byteOffset = samplesOffset * bytesPerSample;
        
        if (byteOffset >= originalChunk->alen) {
            std::cerr << "createChunkWithOffset(): Offset beyond the end of the sound: " << std::endl;
            return nullptr;
        }
        
        Mix_Chunk* offsetChunk = new Mix_Chunk;
        offsetChunk->allocated = 0; // We don't own the memory
        offsetChunk->abuf = originalChunk->abuf + byteOffset;
        offsetChunk->alen = originalChunk->alen - byteOffset;
        offsetChunk->volume = originalChunk->volume;
        
        return offsetChunk;
    }

    Mix_Chunk* loadFromCache(const std::string& textureName){
        auto it = cache.find(textureName);
        if (it != cache.end()) {
            // std::cout << "loadFromCache: " << textureName << std::endl;
            return it->second;
        }

        return nullptr;
    }

};
