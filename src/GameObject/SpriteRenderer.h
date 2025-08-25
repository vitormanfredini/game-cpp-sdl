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
            std::cerr << "SpriteRenderer: no texture" << std::endl;
            return;
        }

        float x = gameObject.x;
        float y = gameObject.y;
        float width = gameObject.width;
        float height = gameObject.height;

        x = x + props.leftOffset;
        y = y + props.topOffset;

        if(alignment != Alignment::UI){
            auto [offsetX, offsetY] = AlignmentUtils::computeOffsets(alignment, width, height);
            x = x - props.cameraPosX + offsetX;
            y = y - props.cameraPosY + offsetY;
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