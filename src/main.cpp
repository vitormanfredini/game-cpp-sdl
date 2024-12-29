#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "SDLUtils.h"
#include "BinaryResourceLoader.h"
#include "PlayableCharacter.h"
#include "Input.h"
#include "DeltaTime.h"
#include "Menu.h"

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("GAME!!!!!1", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture_rabisco = BinaryResourceLoader::toTexture(renderer, "images/rabisco.png");
    SDL_Texture* texture_menu = BinaryResourceLoader::toTexture(renderer, "images/menu.png");

    PlayableCharacter mainChar;
    mainChar.setTexture(texture_rabisco);
    mainChar.setPosition(220,80);

    Menu menu;
    menu.setTexture(texture_menu);
    
    Input input;
    DeltaTime deltaTime;

    SDL_Event event;
    bool running = true;

    bool paused = false;

    while (running) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    input.handleKeyDown(event.key.keysym.sym);
                    menu.handleKeyDown(event.key.keysym.sym);
                    if(!paused && event.key.keysym.sym==SDLK_ESCAPE){
                        paused = true;
                    }
                    if(paused && event.key.keysym.sym==SDLK_RETURN){
                        paused = false;
                    }
                    break;
                case SDL_KEYUP:
                    input.handleKeyUp(event.key.keysym.sym);
                    break;
            }
        }

        deltaTime.update();
        auto deltaTimeInfo = deltaTime.getDeltaTimeInfo();

        if(!paused){
            input.update(deltaTimeInfo.deltaTime);
            mainChar.move(input.getMovementDirections(), deltaTimeInfo.multiplier);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        
        mainChar.draw(renderer);
        if(paused){
            menu.draw(renderer);
        }

        SDL_RenderPresent(renderer);

        //running = false;
    }

    SDL_DestroyTexture(texture_rabisco);
    SDL_DestroyTexture(texture_menu);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}