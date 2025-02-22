#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "GameObjectRenderers/UIRenderer.h"

class HealthBar: public IRenderable {

public:

    HealthBar(SDL_Texture* foregroundTexture, SDL_Texture* backgroundTexture): foregroundTexture(foregroundTexture), backgroundTexture(backgroundTexture) {
        //
    }

    void setHealth(float newHealth) {
        health = newHealth;
    }

    void render(RenderProps renderProps) override {

        float x = getX();
        float y = getY();
        float width = getWidth();
        float height = getHeight();
    
        SDL_Rect rect = {
            static_cast<int>(std::round(x * renderProps.screenScale)),
            static_cast<int>(std::round(y * renderProps.screenScale)),
            static_cast<int>(std::round(width * renderProps.screenScale)),
            static_cast<int>(std::round(height * renderProps.screenScale)),
        };
        SDL_RenderCopy(renderProps.sdl_renderer, backgroundTexture, NULL, &rect);

        rect = {
            static_cast<int>(std::round(x * renderProps.screenScale)),
            static_cast<int>(std::round(y * renderProps.screenScale)),
            static_cast<int>(std::round(health * width * renderProps.screenScale)),
            static_cast<int>(std::round(height * renderProps.screenScale)),
        };
        SDL_RenderCopy(renderProps.sdl_renderer, foregroundTexture, NULL, &rect);
    };

private:

    SDL_Texture* backgroundTexture = nullptr;
    SDL_Texture* foregroundTexture = nullptr;
    float health = 1.0f;

};
