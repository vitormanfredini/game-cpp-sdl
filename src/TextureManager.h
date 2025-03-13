#pragma once

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "BinaryResourceLoader.h"
#include "SDLUtils.h"
#include "GameObject/GameObject.h"
#include "RenderProps.h"
#include <string> 

class TextureManager {

public:

    TextureManager(SDL_Renderer* sdl_renderer): sdl_renderer(sdl_renderer) { };

    ~TextureManager(){
        clearTexturesCache();
    }

    SDL_Texture* loadTexture(const std::string& filename){

        SDL_Texture* cachedTexture = loadFromCache(filename);
        if(cachedTexture != nullptr){
            return cachedTexture;
        }

        SDL_Texture* texture = BinaryResourceLoader::toTexture(sdl_renderer, filename.c_str());
        if (texture) {
            texturesCache[filename] = texture;
        }
        return texture;

    }

    SDL_Texture* loadTexture(int r, int g, int b){

        std::string textureName = rgbTextureName(r,g,b);

        SDL_Texture* cachedTexture = loadFromCache(textureName);
        if(cachedTexture != nullptr){
            return cachedTexture;
        }

        SDL_Texture* newTexture = SDLUtils::textureFromRGB(sdl_renderer, r, g, b);
        texturesCache[textureName] = newTexture;
        return newTexture;
    }

    void clearTexturesCache() {
        for (auto& pair : texturesCache) {
            SDL_DestroyTexture(pair.second);
        }
        texturesCache.clear();
    }

private:
    SDL_Renderer* sdl_renderer;
    std::unordered_map<std::string, SDL_Texture*> texturesCache;

    std::string rgbTextureName(int r, int g, int b){
        return std::to_string(r) + "_" + std::to_string(g) + "_" + std::to_string(b);
    }

    SDL_Texture* loadFromCache(const std::string& textureName){
        auto it = texturesCache.find(textureName);
        if (it != texturesCache.end()) {
            return it->second;
        }

        return nullptr;
    }

};
