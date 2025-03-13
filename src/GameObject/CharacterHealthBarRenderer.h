#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "../RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"
#include "Character/Character.h"

class CharacterHealthBarRenderer : public RenderComponent {
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* foregroundTexture;

public:
CharacterHealthBarRenderer(SDL_Texture* backgroundTexture, SDL_Texture* foregroundTexture) : backgroundTexture(backgroundTexture), foregroundTexture(foregroundTexture) {}

    void render(GameObject& gameObject, RenderProps props) override {

        Character* character = dynamic_cast<Character*>(&gameObject);
        if (!character) return;

        float healthPercentage = character->getHealthPercentage();

        float x = gameObject.x;
        float y = gameObject.y;
        float width = 0.03;
        float height = 0.003;

        x = x - props.cameraPosX + props.leftOffset - (width / 2);
        y = y - props.cameraPosY + props.topOffset - gameObject.height;

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
            static_cast<int>(std::round(healthPercentage * width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, foregroundTexture, NULL, &rect);

    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<CharacterHealthBarRenderer>(backgroundTexture, foregroundTexture);
    }

};