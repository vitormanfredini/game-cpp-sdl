#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"

class SpriteRenderer : public RenderComponent {
private:
    SDL_Texture* texture;
    Alignment alignment;

public:
    SpriteRenderer(SDL_Texture* texture, Alignment alignment) : texture(texture), alignment(alignment) {}

    void render(GameObject& gameObject, RenderProps props) override {
        if (texture == nullptr) {
            std::cerr << "no texture" << std::endl;
            return;
        }

        float x = gameObject.x;
        float y = gameObject.y;
        float width = gameObject.width;
        float height = gameObject.height;

        switch (alignment) {
            case Alignment::BottomUpCentered:
                x = x - props.cameraPosX + props.leftOffset - (width / 2);
                y = y - props.cameraPosY + props.topOffset - height;
                break;
            case Alignment::Centered:
                x = x - props.cameraPosX + props.leftOffset - (width / 2);
                y = y - props.cameraPosY + props.topOffset - (height / 2);
                break;
            case Alignment::TopLeft:
                x = x - props.cameraPosX + props.leftOffset;
                y = y - props.cameraPosY + props.topOffset;
                break;
            case Alignment::BottomLeft:
                x = x - props.cameraPosX + props.leftOffset;
                y = y - props.cameraPosY + props.topOffset - height;
                break;
            case Alignment::TopDownCentered:
                x = x - props.cameraPosX + props.leftOffset - (width / 2);
                y = y - props.cameraPosY + props.topOffset;
                break;
            case Alignment::UI:
                x = x + props.leftOffset;
                y = y + props.topOffset;
                break;

        }

        SDL_Rect rect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };

        SDL_RenderCopy(props.sdl_renderer, texture, NULL, &rect);
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<SpriteRenderer>(texture, alignment);
    }
};