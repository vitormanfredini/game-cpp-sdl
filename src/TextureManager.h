#pragma once

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "BinaryResourceLoader.h"
#include "SDLUtils.h"
#include "GameObject/GameObject.h"
#include "RenderProps.h"
#include <string>
#include <sstream>

class TextureManager {

public:

    TextureManager(SDL_Renderer* sdl_renderer, TTF_Font* firaSansRegular24ptFont): sdl_renderer(sdl_renderer), firaSansRegular24ptFont(firaSansRegular24ptFont) { };

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

    SDL_Texture* loadTexture(int r, int g, int b, int a){

        std::string textureName = rgbaTextureName(r,g,b,a);

        SDL_Texture* cachedTexture = loadFromCache(textureName);
        if(cachedTexture != nullptr){
            return cachedTexture;
        }

        SDL_Texture* newTexture = SDLUtils::textureFromRGBA(sdl_renderer, r, g, b, a);
        texturesCache[textureName] = newTexture;
        return newTexture;
    }

    SDL_Texture* loadTexture(int r, int g, int b){
        return loadTexture(r, g, b, 255);
    }

    SDL_Texture* drawTextOnTexture(SDL_Texture* originalTexture, const char *text, bool copyOriginalTexture) {

        std::string textureName = "";

        if(copyOriginalTexture){
            std::ostringstream oss;
            oss << text << '_' << originalTexture;
            textureName = oss.str();

            SDL_Texture* cachedTexture = loadFromCache(textureName);
            if(cachedTexture != nullptr){
                return cachedTexture;
            }
        }

        SDL_Texture* textureToDrawOn = originalTexture;

        if(copyOriginalTexture){
            Uint32 fmt;
            int access, width, height;
            SDL_QueryTexture(originalTexture, &fmt, &access, &width, &height);
            SDL_Texture* copyTex = SDL_CreateTexture(
                sdl_renderer,
                fmt,
                SDL_TEXTUREACCESS_TARGET,
                width, height
            );
            textureToDrawOn = copyTex;
        }

        SDL_SetTextureBlendMode(textureToDrawOn, SDL_BLENDMODE_BLEND);

        // change renderer to the new texture
        SDL_SetRenderTarget(sdl_renderer, textureToDrawOn);

        // copy texture from original to the copy
        if(copyOriginalTexture){
            SDL_RenderCopy(sdl_renderer, originalTexture, nullptr, nullptr);
        }

        // create texture with text
        SDL_Texture* textureWithText = createTextTexture(text);

        // copy texture over copied texture
        SDL_RenderCopy(sdl_renderer, textureWithText, nullptr, nullptr);

        SDL_SetRenderTarget(sdl_renderer, nullptr);

        if(copyOriginalTexture){
            texturesCache[textureName] = textureToDrawOn;
        }

        return textureToDrawOn;
    }

    SDL_Texture* createTextTexture(const char* text) {

        SDL_Color red = { 255, 30, 30, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Blended(firaSansRegular24ptFont, text, red);
        if (!textSurface) {
            SDL_Log("TTF_RenderText_Blended failed: %s", TTF_GetError());
            return nullptr;
        }
    
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdl_renderer, textSurface);
        SDL_FreeSurface(textSurface);
    
        if (!textTexture) {
            SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
            return nullptr;
        }
    
        SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
    
        return textTexture;
    }

    void clearTexturesCache() {
        for (auto& pair : texturesCache) {
            SDL_DestroyTexture(pair.second);
        }
        texturesCache.clear();
    }

private:
    SDL_Renderer* sdl_renderer;
    TTF_Font* firaSansRegular24ptFont;
    std::unordered_map<std::string, SDL_Texture*> texturesCache;

    std::string rgbaTextureName(int r, int g, int b, int a){
        return std::to_string(r) + "_" + std::to_string(g) + "_" + std::to_string(b) + "_" + std::to_string(a);
    }

    SDL_Texture* loadFromCache(const std::string& textureName){
        auto it = texturesCache.find(textureName);
        if (it != texturesCache.end()) {
            std::cout << "loadFromCache: " << textureName << std::endl;
            return it->second;
        }

        return nullptr;
    }

};
