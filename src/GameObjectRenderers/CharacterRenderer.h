#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "../RenderProps.h"
#include "../interfaces/IRenderable.h"

class CharacterRenderer {
public:

    static void render(RenderProps props, IRenderable* renderable) {
        SDL_Texture* texture = renderable->getTexture();
        if (texture == nullptr){
            std::cerr << "no texture" << std::endl;
            return;
        }

        float x = renderable->getX();
        float y = renderable->getY();
        float width = renderable->getWidth();
        float height = renderable->getHeight();

        x = x - props.cameraPosX + props.leftOffset - (width / 2);
        y = y - props.cameraPosY + props.topOffset - height;

        SDL_Rect rect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };

        SDL_RenderCopy(props.sdl_renderer, texture, NULL, &rect);
    }

};