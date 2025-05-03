#pragma once

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "BinaryResourceLoader.h"
#include "RenderProps.h"

class Renderer {

public:

    Renderer(SDL_Renderer* sdl_renderer, int _widthPixels, int _heightPixels) : sdl_renderer(sdl_renderer), widthPixels(_widthPixels), heightPixels(_heightPixels), window(nullptr) {
        screenScale = widthPixels > heightPixels ? static_cast<float>(widthPixels) : static_cast<float>(heightPixels);

        if(widthPixels > heightPixels){
            float ratio = static_cast<float>(heightPixels) / static_cast<float>(widthPixels);
            topOffset = ((1.0f - ratio) / 2) * -1;
        }else{
            float ratio = static_cast<float>(widthPixels) / static_cast<float>(heightPixels);
            leftOffset = ((1.0f - ratio) / 2) * -1;
        }
    };

    ~Renderer(){ }

    void clearRenderables(){
        renderables.clear();
    }

    void addRenderable(GameObject* renderable){
        renderables.push_back(renderable);
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

    void getVirtualMouseCoords(float* mouseX, float* mouseY){
        int mouseScreenX, mouseScreenY;
        SDL_GetMouseState( &mouseScreenX, &mouseScreenY );
        *mouseX = (static_cast<float>(mouseScreenX) / screenScale) - leftOffset;
        *mouseY = (static_cast<float>(mouseScreenY) / screenScale) - topOffset;
    }

private:
    int widthPixels;
    int heightPixels;
   
    float screenScale;

    float leftOffset = 0.0f;
    float topOffset = 0.0f;

    SDL_Window* window = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;

    std::vector<GameObject*> renderables = {};

};
