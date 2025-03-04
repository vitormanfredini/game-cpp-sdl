#pragma once

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "BinaryResourceLoader.h"
#include "SDLUtils.h"
#include "interfaces/IRenderable.h"
#include "RenderProps.h"

class Renderer {

private:
    int widthPixels;
    int heightPixels;
    
    float screenScale;

    float leftOffset = 0.0f;
    float topOffset = 0.0f;

    SDL_Window* window;
    SDL_Renderer* sdl_renderer;

    std::vector<IRenderable*> renderables = {};

    std::unordered_map<std::string, SDL_Texture*> texturesCache;

    

public:

    Renderer(int _widthPixels, int _heightPixels) : widthPixels(_widthPixels), heightPixels(_heightPixels), window(nullptr), sdl_renderer(nullptr) {
        screenScale = _widthPixels > _heightPixels ? static_cast<float>(_widthPixels) : static_cast<float>(_heightPixels);

        if(widthPixels > heightPixels){
            float ratio = static_cast<float>(heightPixels) / static_cast<float>(widthPixels);
            topOffset = ((1.0f - ratio) / 2) * -1;
        }else{
            float ratio = static_cast<float>(widthPixels) / static_cast<float>(heightPixels);
            leftOffset = ((1.0f - ratio) / 2) * -1;
        }
    };

    ~Renderer(){
        clearTexturesCache();
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    }

    void clearRenderables(){
        renderables.clear();
    }

    void addRenderable(IRenderable* renderable){
        renderables.push_back(renderable);
    }

    bool initialize(){

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        window = SDL_CreateWindow("GAME!!!!!1", 100, 100, widthPixels, heightPixels, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            IMG_Quit();
            SDL_Quit();
            return false;
        }

        sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (sdl_renderer == nullptr) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return false;
        }

        return true;
    }

    void render(float cameraPosX, float cameraPosY){

        SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
        SDL_RenderClear(sdl_renderer);

        cameraPosX = cameraPosX - 0.5f;
        cameraPosY = cameraPosY - 0.5f;

        RenderProps props {
            sdl_renderer,
            cameraPosX,
            cameraPosY,
            topOffset,
            leftOffset,
            screenScale
        };

        for (size_t i = 0; i < renderables.size(); ++i) {
            renderables[i]->render(props);
        }

        SDL_RenderPresent(sdl_renderer);
    }

    SDL_Texture* loadTexture(const std::string& filename){

        auto it = texturesCache.find(filename);
        if (it != texturesCache.end()) {
            return it->second;
        }

        SDL_Texture* texture = BinaryResourceLoader::toTexture(sdl_renderer, filename.c_str());
        if (texture) {
            texturesCache[filename] = texture;
        }
        return texture;

    }

    SDL_Texture* loadTexture(int r, int g, int b){
        return SDLUtils::textureFromRGB(sdl_renderer, r, g, b);
    }

    void clearTexturesCache() {
        for (auto& pair : texturesCache) {
            SDL_DestroyTexture(pair.second);
        }
        texturesCache.clear();
    }

};
