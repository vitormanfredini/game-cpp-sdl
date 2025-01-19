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
    SDL_Window* window;
    SDL_Renderer* sdl_renderer;

    // SDL_Texture* texture_dog;
    // SDL_Texture* texture_enemy;
    // SDL_Texture* texture_menu;
    // SDL_Texture* texture_projectile;
    // SDL_Texture* texture_healthbar;

public:

    Renderer(int _width, int _height) : width(_width), height(_height), window(nullptr), sdl_renderer(nullptr) {};

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
        
        // mainChar.render(sdl_renderer);
        // for(Character& enemy : enemies){
        //     enemy.render(sdl_renderer);
        // }
        // healthBar.render(sdl_renderer);
        // for(Projectile& projectile : projectiles){
        //     projectile.render(sdl_renderer);
        // }
        // if(paused){
        //     menu.render(sdl_renderer);
        // }

        for (size_t i = 0; i < renderables.size(); ++i) {
            renderables[i]->render(sdl_renderer);
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
