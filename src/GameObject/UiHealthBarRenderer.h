#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "../RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"
#include "Character/Character.h"

class UiHealthBarRenderer : public RenderComponent {
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* foregroundTexture;
    Character* characterPtr;

public:
UiHealthBarRenderer(SDL_Texture* backgroundTexture, SDL_Texture* foregroundTexture, Character* characterPtr) : backgroundTexture(backgroundTexture), foregroundTexture(foregroundTexture), characterPtr(characterPtr) {}

    void render(GameObject& gameObject, RenderProps props) override {

        if (!characterPtr) return;

        float healthPercentage = characterPtr->getHealthPercentage();

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
            static_cast<int>(std::round(healthPercentage * width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, foregroundTexture, NULL, &rect);

    }

    void setCharacterPtr(Character* newCharacterPtr){
        characterPtr = newCharacterPtr;
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<UiHealthBarRenderer>(backgroundTexture, foregroundTexture, characterPtr);
    }

};