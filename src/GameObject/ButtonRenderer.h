#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Ui/Button.h"
#include "Ui/ButtonState.h"

class ButtonRenderer : public RenderComponent {
private:
    SDL_Texture* texture;
    int textureWidth, textureHeight;

public:
    ButtonRenderer(SDL_Texture* texture) : texture(texture) {
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
    }

    void render(GameObject& gameObject, RenderProps props) override {

        Button* button = dynamic_cast<Button*>(&gameObject);
        if (!button){
            std::cerr << "ButtonRenderer: not attached to a Button" << std::endl;
            return;
        }

        if (texture == nullptr) {
            std::cerr << "ButtonRenderer: no texture" << std::endl;
            return;
        }

        float x = gameObject.x + props.leftOffset;
        float y = gameObject.y + props.topOffset;
        float width = gameObject.width;
        float height = gameObject.height;

        int spritePartHeight = textureHeight / 3;

        int srcTextureY = 0;
        switch (button->getCurrentState()) {
            case ButtonState::Idle:
                break;
            case ButtonState::Hover:
                srcTextureY += spritePartHeight;
                break;
            case ButtonState::Pressed:
                srcTextureY += spritePartHeight * 2;
                break;
        }

        SDL_Rect srcrect = {
            0,
            srcTextureY,
            textureWidth,
            spritePartHeight,
        };

        SDL_Rect dstrect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };

        SDL_RenderCopy(props.sdl_renderer, texture, &srcrect, &dstrect);
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<ButtonRenderer>(texture);
    }
};