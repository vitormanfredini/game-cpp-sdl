#pragma once

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "BinaryResourceLoader.h"
#include "SDLUtils.h"
#include "GameObject/GameObject.h"
#include "RenderProps.h"
#include "Font/FontManager.h"
#include <string>
#include <sstream>

enum class TextRenderMethod {
    Centered,
    ButtonCentered,
};

class TextureManager {

public:

    TextureManager(SDL_Renderer* sdl_renderer, FontManager* fontManager): sdl_renderer(sdl_renderer), fontManager(fontManager) { };

    ~TextureManager(){
        clearCache();
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

    SDL_Texture* drawTextOnTexture(SDL_Texture* originalTexture, const char *text, FontStyle fontStyle, SDL_Color* color, TextRenderMethod method) {

        bool copyOriginalTexture = true;

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

        int originalTextureWidth, originalTextureHeight;
        Uint32 originalFormat;
        SDL_QueryTexture(originalTexture, &originalFormat, nullptr, &originalTextureWidth, &originalTextureHeight);

        if(copyOriginalTexture){
            SDL_Texture* copyTex = SDL_CreateTexture(
                sdl_renderer,
                originalFormat,
                SDL_TEXTUREACCESS_TARGET,
                originalTextureWidth,
                originalTextureHeight
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
        SDL_Texture* textureWithText = createTextTexture(text, fontStyle, color);

        int textTextureWidth, textTextureHeight;
        SDL_QueryTexture(textureWithText, nullptr, nullptr, &textTextureWidth, &textTextureHeight);

        std::vector<SDL_Rect> renderRects = {};
        switch (method) {
            case TextRenderMethod::Centered: {
                int textX = std::round(static_cast<float>(originalTextureWidth)/2 - static_cast<float>(textTextureWidth)/2);
                int textY = std::round(static_cast<float>(originalTextureHeight)/2 - static_cast<float>(textTextureHeight)/2);
                SDL_Rect rect = {
                    textX,
                    textY,
                    textTextureWidth,
                    textTextureHeight,
                };
                renderRects.push_back(rect);
            }
            break;
            case TextRenderMethod::ButtonCentered: {
                int xAllStates = std::round(static_cast<float>(originalTextureWidth)/2 - static_cast<float>(textTextureWidth)/2);
                int yCenteredFirstState = std::round(static_cast<float>(originalTextureHeight)/6 - static_cast<float>(textTextureHeight)/2);
                int heightEachState = std::round(static_cast<float>(originalTextureHeight)/3);
                
                for(int c=0;c<3;c++){
                    int offset = c==2 ? (textTextureWidth / 40) : 0;
                    SDL_Rect rect = {
                        xAllStates + offset,
                        yCenteredFirstState + (c * heightEachState) + offset,
                        textTextureWidth,
                        textTextureHeight,
                    };
                    renderRects.push_back(rect);
                }
            }
            break;
            default:
                std::cerr << "drawTextOnTexture: unsupported TextRenderMethod" << std::endl;
                break;
        }

        for(SDL_Rect& rect : renderRects){
            SDL_RenderCopy(sdl_renderer, textureWithText, nullptr, &rect);
        }

        SDL_SetRenderTarget(sdl_renderer, nullptr);

        if(copyOriginalTexture){
            texturesCache[textureName] = textureToDrawOn;
        }

        return textureToDrawOn;
    }

    void clearCache() {
        for (auto& pair : texturesCache) {
            SDL_DestroyTexture(pair.second);
        }
        texturesCache.clear();
    }

private:
    SDL_Renderer* sdl_renderer;
    FontManager* fontManager;
    std::unordered_map<std::string, SDL_Texture*> texturesCache;

    std::string rgbaTextureName(int r, int g, int b, int a){
        return std::to_string(r) + "_" + std::to_string(g) + "_" + std::to_string(b) + "_" + std::to_string(a);
    }

    SDL_Texture* loadFromCache(const std::string& textureName){
        auto it = texturesCache.find(textureName);
        if (it != texturesCache.end()) {
            // std::cout << "loadFromCache: " << textureName << std::endl;
            return it->second;
        }

        return nullptr;
    }

    SDL_Texture* createTextTexture(const char* text, FontStyle fontStyle, SDL_Color* color) {

        TTF_Font* font = fontManager->getFont(fontStyle);
        if(font == nullptr){
            std::cerr << "createTextTexture(): could not load font" << std::endl;
            return nullptr;
        }

        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, *color);
        if (!textSurface) {
            SDL_Log("TTF_RenderUTF8_Blended failed: %s", TTF_GetError());
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

};
