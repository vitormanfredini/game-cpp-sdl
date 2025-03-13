#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "GameObject/GameObject.h"

class Menu: public GameObject {

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

};
