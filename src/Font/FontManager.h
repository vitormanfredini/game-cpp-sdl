#pragma once

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "BinaryResourceLoader.h"
#include "SDLUtils.h"
#include "GameObject/GameObject.h"
#include "RenderProps.h"
#include <string>
#include <sstream>
#include "FontStyle.h"

class FontManager {

public:

    FontManager(){
        TTF_Init();

        fontSpecs[FontStyle::MainMenu] = {
            "fonts/Cousine_Regular.ttf",
            32
        };

        fontSpecs[FontStyle::UpgradeMenu] = {
            "fonts/Cousine_Regular.ttf",
            22
        };
    }

    ~FontManager(){
        clearFontsCache();
    }

    TTF_Font* getFont(FontStyle fontStyle){

        auto it = fontSpecs.find(fontStyle);
        if (it == fontSpecs.end()) {
            std::cerr << "fontStyle doesnt exist." << std::endl;
            return nullptr;
        }

        FontRenderSpecs fontSpec = fontSpecs[fontStyle];

        std::string cacheName = getCacheName(fontSpec.filename, fontSpec.size_in_pt);

        TTF_Font* loadedFromCache = loadFromCache(cacheName);
        if (loadedFromCache != nullptr) {
            return loadedFromCache;
        }

        TTF_Font* loaded = loadFont(BinaryResourceLoader::getBinaryResource(fontSpec.filename.c_str()), fontSpec.size_in_pt);
        if (!loaded) {
            std::cerr << "could not loadFont: " << fontSpec.filename << std::endl;
            return nullptr;
        }

        cache[cacheName] = loaded;
        return loaded;
    }

    void clearFontsCache() {
        for (auto& pair : cache) {
            TTF_CloseFont(pair.second);
        }
        cache.clear();
    }

private:

    struct FontRenderSpecs {
        std::string filename;
        int size_in_pt;
    };

    std::unordered_map<FontStyle, FontRenderSpecs> fontSpecs;
    
    std::unordered_map<std::string, TTF_Font*> cache;

    std::string getCacheName(std::string filename, int pt){
        return filename + "_" + std::to_string(pt);
    }

    TTF_Font* loadFromCache(const std::string& cacheName){
        auto it = cache.find(cacheName);
        if (it != cache.end()) {
            // std::cout << "loadFromCache: " << cacheName << std::endl;
            return it->second;
        }

        return nullptr;
    }

    TTF_Font* loadFont(BinaryResource fontBinaryResource, int pt){
        SDL_RWops* rw = SDL_RWFromConstMem(fontBinaryResource.data, (int) fontBinaryResource.length);
        if (!rw) {
            SDL_Log("RWFromConstMem failed: %s", SDL_GetError());
            return nullptr;
        }

        TTF_Font* newFont = TTF_OpenFontRW(rw, /*freesrc=*/1, /*ptSize=*/pt);
        if (!newFont) {
            SDL_Log("TTF_OpenFontRW failed: %s", TTF_GetError());
            return nullptr;
        }

        return newFont;
    }

};
