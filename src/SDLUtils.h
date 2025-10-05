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

    static SDL_Texture* loadTextureFromEmbedded(SDL_Renderer* renderer, const unsigned char* data, unsigned int length) {
        SDL_Surface* surface = loadSurfaceFromEmbedded(renderer, data, length);

        if (!surface) {
            std::cerr << "IMG_Load_RW Error: " << IMG_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }

    static SDL_Surface* loadSurfaceFromEmbedded(SDL_Renderer* renderer, const unsigned char* data, unsigned int length) {
        SDL_RWops* rw = SDL_RWFromConstMem(data, length);
        if (!rw) {
            std::cerr << "SDL_RWFromConstMem Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        return IMG_Load_RW(rw, 1);
    }

    static SDL_Texture* textureFromRGBA(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int width, int height) {

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

        std::vector<std::vector<RGBAPixel>> pixelData;

        Uint8* pixels = (Uint8*)surface->pixels;
        int bpp = surface->format->BytesPerPixel;

        for (int y = 0; y < surface->h; y++) {
            pixelData.push_back({});
            for (int x = 0; x < surface->w; x++) {
                Uint8 r, g, b, a;
                Uint32 pixel = 0;

                // Get pixel address
                Uint8* p = pixels + y * surface->pitch + x * bpp;

                switch (bpp) {
                    case 1: // 8-bit
                        pixel = *p;
                        break;
                    case 2: // 16-bit
                        pixel = *(Uint16*)p;
                        break;
                    case 3: // 24-bit
                        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                            pixel = p[0] << 16 | p[1] << 8 | p[2];
                        else
                            pixel = p[2] << 16 | p[1] << 8 | p[0];
                        break;
                    case 4: // 32-bit
                        pixel = *(Uint32*)p;
                        break;
                }

                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

                pixelData[y].push_back({(int)r, (int)g, (int)b, (int)a});

                // std::cout << "Pixel at (" << x << ", " << y << ") - "
                //         << "R: " << (int)r << ", G: " << (int)g
                //         << ", B: " << (int)b << ", A: " << (int)a << std::endl;
            }
        }

        SDL_UnlockSurface(surface);

        return pixelData;
    }

    static void saveTextureToBMP(SDL_Renderer* renderer, SDL_Texture* texture, const std::string& filename) {
        if (!texture || !renderer) {
            std::cerr << "Invalid texture or renderer!" << std::endl;
            return;
        }

        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
        if (!surface) {
            std::cerr << "Failed to create surface! SDL Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_SetRenderTarget(renderer, texture);
        if (SDL_RenderReadPixels(renderer, nullptr, surface->format->format, surface->pixels, surface->pitch) != 0) {
            std::cerr << "Failed to read texture pixels! SDL Error: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }
        SDL_SetRenderTarget(renderer, nullptr);

        if (SDL_SaveBMP(surface, filename.c_str()) != 0) {
            std::cerr << "Failed to save BMP file! SDL Error: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }

        SDL_FreeSurface(surface);
    }

};
