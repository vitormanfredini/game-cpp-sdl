#pragma once

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "BinaryResourceLoader.h"
#include "SDLUtils.h"
#include "interfaces/IRenderable.h"
#include "RenderProps.h"

class Renderer {

public:

    Renderer(int _widthPixels, int _heightPixels) : widthPixels(_widthPixels), heightPixels(_heightPixels), window(nullptr), sdl_renderer(nullptr) {
        screenScale = widthPixels > heightPixels ? static_cast<float>(widthPixels) : static_cast<float>(heightPixels);

        if(widthPixels > heightPixels){
            float ratio = static_cast<float>(heightPixels) / static_cast<float>(widthPixels);
            topOffset = ((1.0f - ratio) / 2) * -1;
        }else{
            float ratio = static_cast<float>(widthPixels) / static_cast<float>(heightPixels);
            leftOffset = ((1.0f - ratio) / 2) * -1;
        }

        initializeSDL();
    };

    ~Renderer(){
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

    bool initializedCorrectly(){
        return sdl_renderer != nullptr;
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

    SDL_Renderer* getSDLRenderer(){
        return sdl_renderer;
    }

private:
    int widthPixels;
    int heightPixels;
    
    float screenScale;

    float leftOffset = 0.0f;
    float topOffset = 0.0f;

    SDL_Window* window = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;

    std::vector<IRenderable*> renderables = {};

    void initializeSDL(){

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
            SDL_Quit();
            return;
        }

        window = SDL_CreateWindow("GAME!!!!!1", 100, 100, widthPixels, heightPixels, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            IMG_Quit();
            SDL_Quit();
            return;
        }

        sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (sdl_renderer == nullptr) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return;
        }

    }

};
