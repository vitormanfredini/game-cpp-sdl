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
        int channel = Mix_PlayChannel(-1, sound, 0); // 0 = play once (not looped)
        if (channel == -1) {
            std::cerr << "Failed to play audio: " << Mix_GetError() << std::endl;
        }
    }

private:
    std::unordered_map<std::string, Mix_Chunk*> cache;

    Mix_Chunk* loadFromCache(const std::string& textureName){
        auto it = cache.find(textureName);
        if (it != cache.end()) {
            // std::cout << "loadFromCache: " << textureName << std::endl;
            return it->second;
        }

        return nullptr;
    }

};
