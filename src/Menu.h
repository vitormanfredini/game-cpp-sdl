#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"

class Menu: public IRenderable {

private:
    SDL_Rect rect = {0, 0, 2*800, 2*600};
    SDL_Texture* texture = nullptr;

    float x = 0.0f;
    float y = 0.0f;

public:

    void handleKeyDown(SDL_Keycode keyCode){
        switch (keyCode) {
            case SDLK_UP:
                break;
            case SDLK_DOWN:
                break;
            case SDLK_LEFT:
                break;
            case SDLK_RIGHT:
                break;
            case SDLK_ESCAPE:
                break;
            case SDLK_RETURN:
                break;
            default:
                break;
        }
    }

    void setTexture(SDL_Texture* _texture){
        texture = _texture;
    }

    void render(SDL_Renderer* renderer) override {
        if(texture == nullptr){
            return;
        }

        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

};
