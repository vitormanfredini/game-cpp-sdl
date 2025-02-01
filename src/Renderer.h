#pragma once

#include "BinaryResourceLoader.h"
#include "Renderer.h"
#include <SDL2/SDL_image.h>
#include "SDLUtils.h"
#include "interfaces/IRenderable.h"
#include <random>

class Renderer {

private:
    int width;
    int height;
    int maxWidthHeight;

    float normalizedMaxWidth;
    float normalizedMaxHeight;

    SDL_Window* window;
    SDL_Renderer* sdl_renderer;

    // SDL_Texture* texture_dog;
    // SDL_Texture* texture_enemy;
    // SDL_Texture* texture_menu;
    // SDL_Texture* texture_projectile;
    // SDL_Texture* texture_healthbar;

public:

    Renderer(int _width, int _height) : width(_width), height(_height), window(nullptr), sdl_renderer(nullptr) {
        if(_width > _height){
            normalizedMaxWidth = 1.0f;
            normalizedMaxHeight = static_cast<float>(_height) / static_cast<float>(_width);
            maxWidthHeight = _width;
        }else{
            normalizedMaxWidth = static_cast<float>(_width) / static_cast<float>(_height);
            normalizedMaxHeight = 1.0f;
            maxWidthHeight = _height;
        }
    };

    ~Renderer(){
        
        // SDL_DestroyTexture(texture_dog);
        // SDL_DestroyTexture(texture_enemy);
        // SDL_DestroyTexture(texture_menu);
        // SDL_DestroyTexture(texture_projectile);
        // SDL_DestroyTexture(texture_healthbar);

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

        for (size_t i = 0; i < renderables.size(); ++i) {
            SDL_Texture* texture = renderables[i]->getTexture();
            if (texture == nullptr){
                continue;
            }
            SDL_Rect rect = {
                static_cast<int>(std::round(renderables[i]->getX() * static_cast<float>(maxWidthHeight))),
                static_cast<int>(std::round(renderables[i]->getY() * static_cast<float>(maxWidthHeight))),
                static_cast<int>(std::round(renderables[i]->getWidth() * static_cast<float>(maxWidthHeight))),
                static_cast<int>(std::round(renderables[i]->getHeight() * static_cast<float>(maxWidthHeight))),
            };
            SDL_RenderCopy(sdl_renderer, texture, NULL, &rect);
        }

        SDL_RenderPresent(sdl_renderer);
    }

    SDL_Texture* loadTexture(const char* filename){
        return BinaryResourceLoader::toTexture(sdl_renderer, filename);
    }

    SDL_Texture* loadTexture(int r, int g, int b){
        return SDLUtils::textureFromRGB(sdl_renderer, r, g, b);
    }

    std::vector<float> getRandomPositionOutsideScreen(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        float x = 0.0f;
        float y = 0.0f;

        if(dis(gen) > 0.5){
            if(dis(gen) > 0.5){
                x = normalizedMaxWidth + 0.1f;
            }else{
                x = -0.1f;
            }
            y = normalizedMaxHeight * dis(gen);
        }else{
            x = normalizedMaxWidth * dis(gen);
            if(dis(gen) > 0.5){
                y = normalizedMaxHeight + 0.1f;
            }else{
                y = -0.1f;
            }
        }

        return {x,y};
    }

    std::vector<float> getPositionCenterOfScreen(){
        return {
            normalizedMaxWidth/2,
            normalizedMaxHeight/2
        };
    }

    float getMaxWidth(){
        return normalizedMaxWidth;
    }

    float getMaxHeight(){
        return normalizedMaxHeight;
    }

};
