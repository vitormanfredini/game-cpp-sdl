#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <SDL_mixer.h>
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

    static SDL_Texture* textureFromRGBA(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {

        SDL_Surface* surface = SDL_CreateRGBSurface(0,64,64,32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
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

        for (int x = 0; x < surface->w; x++) {
            pixelData.push_back({});
            for (int y = 0; y < surface->h; y++) {
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

                pixelData[x].push_back({(int)r, (int)g, (int)b, (int)a});

                // std::cout << "Pixel at (" << x << ", " << y << ") - "
                //         << "R: " << (int)r << ", G: " << (int)g
                //         << ", B: " << (int)b << ", A: " << (int)a << std::endl;
            }
        }

        SDL_UnlockSurface(surface);

        return pixelData;
    }

    static void initializeSDL(SDL_Window*& window, SDL_Renderer*& sdl_renderer, int width, int height){

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return;
        }
    
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
            SDL_Quit();
            return;
        }
    
        // if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        //     std::cerr << "SDL_mixer Error: " << Mix_GetError() << std::endl;
        //     IMG_Quit();
        //     SDL_Quit();
        //     return;
        // }
    
        window = SDL_CreateWindow("GAME!!!!!1", 250, 130, width, height, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            IMG_Quit();
            SDL_Quit();
            // Mix_Quit();
            return;
        }
    
        sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (sdl_renderer == nullptr) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            // Mix_Quit();
            return;
        }
    }

    static void quitSDL(SDL_Window*& window, SDL_Renderer*& sdl_renderer){
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        // Mix_Quit();
    }

};
