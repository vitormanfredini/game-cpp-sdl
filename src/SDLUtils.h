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

    static SDL_Texture* textureFromRGB(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b) {

        SDL_Surface* surface = SDL_CreateRGBSurface(0,64,64,32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        Uint32 pink = SDL_MapRGB(surface->format, r, g, b);
        SDL_FillRect(surface, NULL, pink);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        return texture;
    }

};
