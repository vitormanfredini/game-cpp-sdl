#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"

class HealthBar: public IRenderable {

private:
    SDL_Rect rect = {0, 0, 800*2, 40};
    SDL_Texture* texture = nullptr;

    float x = 0.0f;
    float y = 0.0f;

    void updateRect(){
        rect.x = round(x);
        rect.y = round(y);
    }

public:

    void setTexture(SDL_Texture* _texture){
        texture = _texture;
    }

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
        updateRect();
    }

    void setHealth(float health) {
        rect.w = round(health * static_cast<float>(800*2));
    }

    void render(SDL_Renderer* renderer) override {
        if(texture == nullptr){
            return;
        }

        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    float getX(){
        return x;
    }

    float getY(){
        return y;
    }
};
