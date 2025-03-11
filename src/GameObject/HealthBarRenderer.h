#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "../RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"

class HealthBarRenderer : public RenderComponent {
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* foregroundTexture;
    float* health;

public:
    HealthBarRenderer(SDL_Texture* backgroundTexture, SDL_Texture* foregroundTexture, float* healthPtr) : backgroundTexture(backgroundTexture), foregroundTexture(foregroundTexture), health(healthPtr) {}

    void render(GameObject& gameObject, RenderProps props) override {

        float x = gameObject.x;
        float y = gameObject.y;
        float width = gameObject.width;
        float height = gameObject.height;

        SDL_Rect rect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, backgroundTexture, NULL, &rect);

        rect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round((*health) * width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, foregroundTexture, NULL, &rect);

    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<HealthBarRenderer>(backgroundTexture, foregroundTexture, health);
    }

};