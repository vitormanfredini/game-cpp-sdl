#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"

class ShadowRenderer : public RenderComponent {
private:
    SDL_Texture* texture;
    float height;

public:
    ShadowRenderer(SDL_Texture* texture, float height) : texture(texture), height(height) {}

    void render(GameObject& gameObject, RenderProps props) override {
        if (texture == nullptr) {
            std::cerr << "ShadowRenderer: no texture" << std::endl;
            return;
        }

        float x = gameObject.x;
        float y = gameObject.y;
        float widthRatio = 2.5;
        float width = height * widthRatio;
        
        auto [offsetX, offsetY] = AlignmentUtils::computeOffsets(Alignment::BottomUpCentered, width, height);
        x = x - props.cameraPosX + offsetX + props.leftOffset;
        y = y - props.cameraPosY + offsetY + props.topOffset;
    
        SDL_Rect rect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };

        SDL_RenderCopy(props.sdl_renderer, texture, NULL, &rect);
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<ShadowRenderer>(texture, height);
    }
};