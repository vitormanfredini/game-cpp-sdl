#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class SDLUtils {

public:

    static SDL_Texture* loadTextureFromEmbedded(SDL_Renderer* renderer, const unsigned char* data, unsigned int length) {
        SDL_RWops* rw = SDL_RWFromConstMem(data, length);
        if (!rw) {
            std::cerr << "SDL_RWFromConstMem Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        SDL_Surface* surface = IMG_Load_RW(rw, 1);
        if (!surface) {
            std::cerr << "IMG_Load_RW Error: " << IMG_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }

};
