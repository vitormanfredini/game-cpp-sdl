#pragma once

#include "BinaryResourceLoader.h"
#include "Renderer.h"
#include <SDL2/SDL_image.h>
#include "SDLUtils.h"
#include "interfaces/IRenderable.h"

class Renderer {

private:
    int widthPixels;
    int heightPixels;
    
    float screenScale;

    float leftOffset = 0.0f;
    float topOffset = 0.0f;

    SDL_Window* window;
    SDL_Renderer* sdl_renderer;

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
        
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
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

    void render(std::vector<IRenderable*>& renderables, std::vector<float> cameraPos){

        SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
        SDL_RenderClear(sdl_renderer);

        float cameraOffsetLeft = cameraPos[0] - 0.5f;
        float cameraOffsetTop = cameraPos[1] - 0.5f;

        for (size_t i = 0; i < renderables.size(); ++i) {
            SDL_Texture* texture = renderables[i]->getTexture();
            if (texture == nullptr){
                continue;
            }

            float x = renderables[i]->getX();
            float y = renderables[i]->getY();
            float width = renderables[i]->getWidth();
            float height = renderables[i]->getHeight();

            if(renderables[i]->getRenderAnchor() == RenderAnchor::GAMEWORLD){
                x = x - cameraOffsetLeft + leftOffset;
                y = y - cameraOffsetTop + topOffset;
            }

            if(renderables[i]->getRenderAnchor() == RenderAnchor::UI_FULLWIDTH_TOP){
                float scaleToFitScreenWidth = renderables[i]->getWidth() / 1.0f;
                width = width * scaleToFitScreenWidth;
                height = height * scaleToFitScreenWidth;
            }

            if(renderables[i]->getRenderAnchor() == RenderAnchor::GAMEWORLD || renderables[i]->getRenderAnchor() == RenderAnchor::UI_FULLWIDTH_TOP){
                SDL_Rect rect = {
                    static_cast<int>(std::round(x * screenScale)),
                    static_cast<int>(std::round(y * screenScale)),
                    static_cast<int>(std::round(width * screenScale)),
                    static_cast<int>(std::round(height * screenScale)),
                };
                SDL_RenderCopy(sdl_renderer, texture, NULL, &rect);
            }
            
        }

        SDL_RenderPresent(sdl_renderer);
    }

    SDL_Texture* loadTexture(const char* filename){
        return BinaryResourceLoader::toTexture(sdl_renderer, filename);
    }

    SDL_Texture* loadTexture(int r, int g, int b){
        return SDLUtils::textureFromRGB(sdl_renderer, r, g, b);
    }

};
