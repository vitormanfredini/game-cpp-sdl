#pragma once

#include "BinaryResourceLoader.h"
#include "Renderer.h"
#include <SDL2/SDL_image.h>
#include "SDLUtils.h"
#include "interfaces/IRenderable.h"

class Renderer {

private:
    int width;
    int height;
    float screenScale;

    SDL_Window* window;
    SDL_Renderer* sdl_renderer;

public:

    Renderer(int _width, int _height) : width(_width), height(_height), window(nullptr), sdl_renderer(nullptr) {
        screenScale = _width > _height ? static_cast<float>(_width) : static_cast<float>(_height);
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

        window = SDL_CreateWindow("GAME!!!!!1", 100, 100, width, height, SDL_WINDOW_SHOWN);
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

    void render(std::vector<IRenderable*>& renderables){

        SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
        SDL_RenderClear(sdl_renderer);

        float leftOffset = 0.0f;
        float topOffset = 0.0f;
        if(width > height){
            float ratio = static_cast<float>(height) / static_cast<float>(width);
            topOffset = ((1.0f - ratio) / 2) * -1;
        }else{
            float ratio = static_cast<float>(width) / static_cast<float>(height);
            leftOffset = ((1.0f - ratio) / 2) * -1;
        }

        for (size_t i = 0; i < renderables.size(); ++i) {
            SDL_Texture* texture = renderables[i]->getTexture();
            if (texture == nullptr){
                continue;
            }

            if(renderables[i]->getRenderAnchor() == RenderAnchor::GAMEWORLD){
                SDL_Rect rect = {
                    static_cast<int>(std::round((renderables[i]->getX() + leftOffset) * screenScale)),
                    static_cast<int>(std::round((renderables[i]->getY() + topOffset) * screenScale)),
                    static_cast<int>(std::round(renderables[i]->getWidth() * screenScale)),
                    static_cast<int>(std::round(renderables[i]->getHeight() * screenScale)),
                };
                SDL_RenderCopy(sdl_renderer, texture, NULL, &rect);
                continue;
            }

            if(renderables[i]->getRenderAnchor() == RenderAnchor::UI_FULLWIDTH_TOP){

                float scaleToFitScreenWidth = renderables[i]->getWidth() / 1.0f;

                SDL_Rect rect = {
                    static_cast<int>(std::round(renderables[i]->getX() * screenScale)),
                    static_cast<int>(std::round(renderables[i]->getY() * screenScale)),
                    static_cast<int>(std::round(renderables[i]->getWidth() * scaleToFitScreenWidth * screenScale)),
                    static_cast<int>(std::round(renderables[i]->getHeight() * scaleToFitScreenWidth * screenScale)),
                };
                SDL_RenderCopy(sdl_renderer, texture, NULL, &rect);
                continue;
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
