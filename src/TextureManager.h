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
    ButtonCentered
};

class TextureManager {

public:

    TextureManager(SDL_Renderer* sdl_renderer, FontManager* fontManager): sdl_renderer(sdl_renderer), fontManager(fontManager) { };

    ~TextureManager(){
        clearCache();
    }

    SDL_Surface* loadSurface(const std::string& filename){

        SDL_Surface* cachedSurface = loadSurfaceFromCache(filename);
        if(cachedSurface != nullptr){
            return cachedSurface;
        }

        BinaryResource binaryResource = BinaryResourceLoader::getBinaryResource(filename.c_str());
        SDL_Surface* surface = SDLUtils::loadSurfaceFromBinary(sdl_renderer, binaryResource.data, binaryResource.length);
        
        if (surface) {
            surfacesCache[filename] = surface;
        }
        return surface;

    }

    SDL_Texture* loadTexture(SDL_Surface* surface, bool useCache = true){

        std::ostringstream oss;
        std::string textureName;

        if(useCache){
            oss << surface << "_textureFromSurface";
            textureName = oss.str();

            SDL_Texture* cachedTexture = loadTextureFromCache(textureName);
            if(cachedTexture != nullptr){
                return cachedTexture;
            }
        }

        SDL_Texture* texture = SDLUtils::loadTextureFromSurface(sdl_renderer, surface);

        if (texture && useCache) {
            texturesCache[textureName] = texture;
        }
        return texture;

    }

    SDL_Texture* loadTexture(const std::string& filename){

        SDL_Texture* cachedTexture = loadTextureFromCache(filename);
        if(cachedTexture != nullptr){
            return cachedTexture;
        }

        SDL_Surface* surface = loadSurface(filename);
        SDL_Texture* texture = SDLUtils::loadTextureFromSurface(sdl_renderer, surface);
        SDL_FreeSurface(surface);
        
        if (texture) {
            texturesCache[filename] = texture;
        }
        return texture;

    }

    SDL_Texture* loadTexture(int r, int g, int b, int a, int width, int height){

        std::string textureName = generateCacheName(r,g,b,a,width,height);

        SDL_Texture* cachedTexture = loadTextureFromCache(textureName);
        if(cachedTexture != nullptr){
            return cachedTexture;
        }

        SDL_Texture* newTexture = SDLUtils::createTextureFromRGBA(sdl_renderer, r, g, b, a, width, height);
        texturesCache[textureName] = newTexture;
        return newTexture;
    }

    SDL_Texture* loadTexture(int r, int g, int b){
        return loadTexture(r, g, b, 255, 1, 1);
    }

    std::vector<std::vector<RGBAPixel>> toRGBAPixelData(const std::string& filename) {
        SDL_Surface* surface = loadSurface(filename);
        std::vector<std::vector<RGBAPixel>> pixels = SDLUtils::getRGBAPixelDataFromSurface(surface);
        SDL_FreeSurface(surface);
        return pixels;
    }

    SDL_Texture* drawTextOnTexture(SDL_Texture* originalTexture, std::string text, FontStyle fontStyle, SDL_Color& color, TextRenderMethod method) {

        bool copyOriginalTexture = true;

        std::string textureName = "";

        if(copyOriginalTexture){
            std::ostringstream oss;
            oss << text << '_' << originalTexture << '_' 
                << static_cast<int>(color.r) << '_'
                << static_cast<int>(color.g) << '_'
                << static_cast<int>(color.b) << '_'
                << static_cast<int>(color.a);
            textureName = oss.str();

            SDL_Texture* cachedTexture = loadTextureFromCache(textureName);
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

        SDL_SetRenderTarget(sdl_renderer, textureToDrawOn);

        if(copyOriginalTexture){
            SDL_RenderCopy(sdl_renderer, originalTexture, nullptr, nullptr);
        }

        SDL_Texture* textureWithText = createTextTexture(text.c_str(), fontStyle, color);

        int textTextureWidth, textTextureHeight;
        SDL_QueryTexture(textureWithText, nullptr, nullptr, &textTextureWidth, &textTextureHeight);

        // std::cout << text << ": " << textTextureWidth  << "x"  << textTextureHeight << std::endl;

        std::vector<SDL_Rect> renderRects = {};
        switch (method) {
            case TextRenderMethod::Centered: {
                int textX = std::round(static_cast<float>(originalTextureWidth)/2 - static_cast<float>(textTextureWidth)/2);
                int textY = std::round(static_cast<float>(originalTextureHeight)/2 - static_cast<float>(textTextureHeight)/2);
                renderRects.push_back({
                    textX,
                    textY,
                    textTextureWidth,
                    textTextureHeight,
                });
            }
            break;
            case TextRenderMethod::ButtonCentered: {
                int xAllStates = std::round(static_cast<float>(originalTextureWidth)/2 - static_cast<float>(textTextureWidth)/2);
                int yFirstState = std::round(static_cast<float>(originalTextureHeight)/6 - static_cast<float>(textTextureHeight)/2);
                // int yFirstState = 0;
                int heightEachState = std::round(static_cast<float>(originalTextureHeight)/3);
                
                for(int c=0;c<3;c++){
                    int offset = c==2 ? (textTextureWidth / 40) : 0;
                    SDL_Rect rect = {
                        xAllStates + offset,
                        yFirstState + (c * heightEachState) + offset,
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

    SDL_Texture* createButtonTexture(
        int buttonWidth,
        int buttonHeight,
        std::string text,
        FontStyle fontStyle,
        SDL_Color& textColorIdle,
        SDL_Color& textColorHover,
        SDL_Color& textColorPressed,
        SDL_Texture* backgroundIdle,
        SDL_Texture* backgroundHover,
        SDL_Texture* backgroundPressed
    ){

        SDL_Texture* idlePart = drawTextOnTexture(
            backgroundIdle,
            text,
            fontStyle,
            textColorIdle,
            TextRenderMethod::Centered
        );

        SDL_Texture* hoverPart = drawTextOnTexture(
            backgroundHover,
            text,
            fontStyle,
            textColorHover,
            TextRenderMethod::Centered
        );

        SDL_Texture* pressedPart = drawTextOnTexture(
            backgroundPressed,
            text,
            fontStyle,
            textColorPressed,
            TextRenderMethod::Centered
        );

        SDL_Texture* idlePlusHover = joinTexturesVertically(idlePart, hoverPart);
        SDL_Texture* completeTexture = joinTexturesVertically(idlePlusHover, pressedPart);

        SDL_DestroyTexture(idlePart);
        SDL_DestroyTexture(hoverPart);
        SDL_DestroyTexture(pressedPart);
        SDL_DestroyTexture(idlePlusHover);

        return completeTexture;
    }

    void loadTextureAndBeenHitTexture(const std::string& filename, SDL_Texture*& texture, SDL_Texture*& beenHitTexture){
        SDL_Surface* surface = loadSurface(filename);
        texture = loadTexture(surface);
        SDLUtils::setSurfaceColorExceptAlpha(surface,191,191,173);
        beenHitTexture = loadTexture(surface, false);
        SDL_FreeSurface(surface);
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
    std::unordered_map<std::string, SDL_Surface*> surfacesCache;

    std::string generateCacheName(int r, int g, int b, int a, int width, int height){
        return std::to_string(r) + "_" + std::to_string(g) + "_" + std::to_string(b) + "_" + std::to_string(a) + "_" + std::to_string(width) + "_" + std::to_string(height);
    }

    SDL_Texture* loadTextureFromCache(const std::string& textureName){
        auto it = texturesCache.find(textureName);
        if (it != texturesCache.end()) {
            // std::cout << "loadTextureFromCache: " << textureName << std::endl;
            return it->second;
        }

        return nullptr;
    }

    SDL_Surface* loadSurfaceFromCache(const std::string& surfaceName){
        auto it = surfacesCache.find(surfaceName);
        if (it != surfacesCache.end()) {
            // std::cout << "loadSurfaceFromCache: " << surfaceName << std::endl;
            return it->second;
        }

        return nullptr;
    }

    SDL_Texture* createTextTexture(const char* text, FontStyle fontStyle, SDL_Color& color) {

        TTF_Font* font = fontManager->getFont(fontStyle);
        if(font == nullptr){
            std::cerr << "createTextTexture(): could not load font" << std::endl;
            return nullptr;
        }

        SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, 0);
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

    SDL_Texture* joinTexturesVertically(SDL_Texture* texture1, SDL_Texture* texture2) {
        if (!texture1 || !texture2) {
            std::cerr << "joinTexturesVertically(): Invalid texture or renderer!" << std::endl;
            return nullptr;
        }

        int width1, height1;
        SDL_QueryTexture(texture1, nullptr, nullptr, &width1, &height1);

        int width2, height2;
        SDL_QueryTexture(texture2, nullptr, nullptr, &width2, &height2);

        int newWidth = std::max(width1, width2);
        int newHeight = height1 + height2;

        SDL_Texture* combinedTexture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, newWidth, newHeight);
        if (!combinedTexture) {
            std::cerr << "Unable to create target texture! SDL Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        SDL_SetTextureBlendMode(texture1, SDL_BLENDMODE_NONE);
        SDL_SetTextureBlendMode(texture2, SDL_BLENDMODE_NONE);
        SDL_SetTextureBlendMode(combinedTexture, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(sdl_renderer, combinedTexture);
        SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
        SDL_RenderClear(sdl_renderer);

        SDL_Rect destRect1 = { 0, 0, width1, height1 };
        SDL_RenderCopy(sdl_renderer, texture1, nullptr, &destRect1);

        SDL_Rect destRect2 = { 0, height1, width2, height2 };
        SDL_RenderCopy(sdl_renderer, texture2, nullptr, &destRect2);

        SDL_SetRenderTarget(sdl_renderer, nullptr);

        return combinedTexture;
    }

};
