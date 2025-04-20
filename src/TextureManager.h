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

    TextureManager(SDL_Renderer* sdl_renderer, TTF_Font* firasansregular12ptfont): sdl_renderer(sdl_renderer), firasansregular12ptfont(firasansregular12ptfont) { };

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
        return loadTexture(r, g, b, 255);
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

    SDL_Texture* loadTextureFromText(const char *text) {
        SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
            0,                // flags
            256, 256,         // width, height
            32,               // depth
            SDL_PIXELFORMAT_RGBA8888
        );
        if (!surf) {
            SDL_Log("SDL_CreateRGBSurfaceWithFormat failed: %s", SDL_GetError());
            return nullptr;
        }

        SDL_FillRect(surf, nullptr,
            SDL_MapRGBA(surf->format, 0, 0, 0, 0)
        );

        SDL_Color white = { 255, 255, 255, 255 };
        SDL_Surface* textSurf = TTF_RenderText_Blended(firasansregular12ptfont, text, white);
        if (!textSurf) {
            SDL_Log("TTF_RenderText_Blended failed: %s", TTF_GetError());
            SDL_FreeSurface(surf);
            return nullptr;
        }

        SDL_Rect dst;
        dst.x = (256 - textSurf->w) / 2;
        dst.y = (256 - textSurf->h) / 2;
        dst.w = textSurf->w;
        dst.h = textSurf->h;
        SDL_BlitSurface(textSurf, nullptr, surf, &dst);
        SDL_FreeSurface(textSurf);

        SDL_Texture* tex = SDL_CreateTextureFromSurface(sdl_renderer, surf);
        if (!tex) {
            SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        }
        SDL_FreeSurface(surf);

        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

        return tex;
    }

    void clearTexturesCache() {
        for (auto& pair : texturesCache) {
            SDL_DestroyTexture(pair.second);
        }
        texturesCache.clear();
    }

private:
    SDL_Renderer* sdl_renderer;
    TTF_Font* firasansregular12ptfont;
    std::unordered_map<std::string, SDL_Texture*> texturesCache;

    std::string rgbaTextureName(int r, int g, int b, int a){
        return std::to_string(r) + "_" + std::to_string(g) + "_" + std::to_string(b) + "_" + std::to_string(a);
    }

    SDL_Texture* loadFromCache(const std::string& textureName){
        auto it = texturesCache.find(textureName);
        if (it != texturesCache.end()) {
            return it->second;
        }

        return nullptr;
    }

};
