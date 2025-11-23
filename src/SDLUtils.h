#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>

struct RGBAPixel {
    int r;
    int g;
    int b;
    int a;
};

class SDLUtils {

public:

    static SDL_Texture* loadTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cerr << "loadTextureFromSurface: SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            return nullptr;
        }
        return texture;
    }

    static SDL_Surface* loadSurfaceFromBinary(SDL_Renderer* renderer, const unsigned char* data, unsigned int length) {
        SDL_RWops* rw = SDL_RWFromConstMem(data, length);
        if (!rw) {
            std::cerr << "loadSurfaceFromBinary: SDL_RWFromConstMem Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        SDL_Surface* loaded = IMG_Load_RW(rw, 1);
        if (!loaded) {
            std::cerr << "loadSurfaceFromBinary: IMG_Load_RW Error: " << IMG_GetError() << std::endl;
            return nullptr;
        }

        // always convert to RGBA32
        SDL_Surface* rgba = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(loaded);

        if (!rgba) {
            std::cerr << "loadSurfaceFromBinary: SDL_ConvertSurfaceFormat Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        return rgba;
    }

    static SDL_Texture* createTextureFromRGBA(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int width, int height) {

        SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        bool noTransparency = a == 255;
        Uint32 color = noTransparency ? SDL_MapRGB(surface->format, r, g, b) : SDL_MapRGBA(surface->format, r, g, b, a);
        SDL_FillRect(surface, NULL, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        return texture;
    }

    static std::vector<std::vector<RGBAPixel>> getRGBAPixelDataFromSurface(SDL_Surface* surface){
        SDL_LockSurface(surface);

        std::cout << SDL_GetPixelFormatName(surface->format->format) << std::endl;

        std::vector<std::vector<RGBAPixel>> pixelData(surface->h);

        for (int y = 0; y < surface->h; y++) {

            pixelData[y].reserve(surface->w);

            Uint32* row = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch);

            for (int x = 0; x < surface->w; x++) {

                Uint32 pixel = row[x];
                Uint8* p = (Uint8*)&pixel;

                Uint8 r = p[0];
                Uint8 g = p[1];
                Uint8 b = p[2];
                Uint8 a = p[3];

                pixelData[y].push_back({ r, g, b, a });
            }
        }

        SDL_UnlockSurface(surface);
        return pixelData;
    }

    static void saveTextureToBMP(SDL_Renderer* renderer, SDL_Texture* texture, const std::string& filename) {
        if (!texture || !renderer) {
            std::cerr << "saveTextureToBMP: Invalid texture or renderer!" << std::endl;
            return;
        }

        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
        if (!surface) {
            std::cerr << "saveTextureToBMP: Failed to create surface! SDL Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_SetRenderTarget(renderer, texture);
        if (SDL_RenderReadPixels(renderer, nullptr, surface->format->format, surface->pixels, surface->pitch) != 0) {
            std::cerr << "saveTextureToBMP: Failed to read texture pixels! SDL Error: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }
        SDL_SetRenderTarget(renderer, nullptr);

        if (SDL_SaveBMP(surface, filename.c_str()) != 0) {
            std::cerr << "saveTextureToBMP: Failed to save BMP file! SDL Error: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }

        SDL_FreeSurface(surface);
    }

};
