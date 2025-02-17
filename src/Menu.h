#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "GameObjectRenderers/UIRenderer.h"

class Menu: public IRenderable {

public:

    void handleKeyDown(SDL_Keycode keyCode){
        switch (keyCode) {
            case SDLK_UP:
                break;
            case SDLK_DOWN:
                break;
            case SDLK_LEFT:
                break;
            case SDLK_RIGHT:
                break;
            case SDLK_ESCAPE:
                break;
            case SDLK_RETURN:
                break;
            default:
                break;
        }
    }

    void render(RenderProps renderProps) override {
        UIRenderer::render(renderProps, this);
    };

};
