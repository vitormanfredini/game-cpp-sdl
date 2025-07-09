#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>

struct BinaryResource {
    unsigned char* data;
    unsigned int length;

    BinaryResource(unsigned char* data, unsigned int length) : data(data), length(length) {}
    BinaryResource() : data(nullptr), length(0) {}
};

class BinaryResourceLoader {

public:

    static BinaryResource getBinaryResource(const char* filename){
        for (int i = 0; i < ResourceRegistry::RESOURCE_COUNT; i++) {
            if (strcmp(ResourceRegistry::RESOURCE_NAMES[i], filename) == 0) {
                return BinaryResource(
                    ResourceRegistry::RESOURCE_DATA[i],
                    ResourceRegistry::RESOURCE_LENGTHS[i]
                );
                break;
            }
        }
        std::cerr << "Binary file " << filename << " could not be found." << std::endl;
        return BinaryResource();
    }

    static SDL_Texture* toTexture(SDL_Renderer *renderer, const char* filename) {

        BinaryResource binaryResource = getBinaryResource(filename);

        SDL_Texture* image_texture = SDLUtils::loadTextureFromEmbedded(renderer, binaryResource.data, binaryResource.length);
        if (image_texture != nullptr) {
            return image_texture;
        }

        std::cerr << "toTexture(): Can't create texture from binary file: " << filename << std::endl;
        std::cerr << "toTexture(): SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        std::cerr << "toTexture(): Using a fallback pink texture." << SDL_GetError() << std::endl;

        SDL_Surface* surface = SDL_CreateRGBSurface(0,64,64,32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        Uint32 pink = SDL_MapRGB(surface->format, 255, 105, 180);
        SDL_FillRect(surface, NULL, pink);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        return texture;
    }

    static std::vector<std::vector<RGBAPixel>> toRGBAPixelData(const char* filename) {
        BinaryResource mapTestImage = BinaryResourceLoader::getBinaryResource(filename);

        SDL_RWops* rw = SDL_RWFromMem(mapTestImage.data, mapTestImage.length);
        if (!rw) {
            std::cerr << "toRGBAPixelData error for file: " << filename << std::endl;
            std::cerr << "SDL_RWFromMem failed: " << SDL_GetError() << std::endl;
            return {{}};
        }

        SDL_Surface* surface = IMG_Load_RW(rw, 1);
        if (!surface) {
            std::cerr << "toRGBAPixelData error for file: " << filename << std::endl;
            std::cerr << "IMG_Load_RW failed: " << IMG_GetError() << std::endl;
            return {{}};
        }

        std::vector<std::vector<RGBAPixel>> pixels = SDLUtils::getRGBAPixelDataFromSurface(surface);

        SDL_FreeSurface(surface);

        return pixels;
    }

};
