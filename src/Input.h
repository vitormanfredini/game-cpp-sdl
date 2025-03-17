#pragma once

#include "resource_registry.h"
#include <iostream>
#include <SDL2/SDL_keycode.h>
#include "MovingAverage.h"
#include "MovementDirection.h"

class Input {

private:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    MovingAverage horizontal {10};
    MovingAverage vertical {10};

public:

    void update(){

        if(up){
            vertical.addValue(-1.0f);
        }else if(down){
            vertical.addValue(1.0f);
        }else{
            vertical.addValue(0.0f);
        }

        if(right){
            horizontal.addValue(1.0f);
        }else if(left){
            horizontal.addValue(-1.0f);
        }else{
            horizontal.addValue(0.0f);
        }
        
    }

    void handleKeyDown(SDL_Keycode keyCode){
        switch (keyCode) {
            case SDLK_UP:
                up = true;
                break;
            case SDLK_DOWN:
                down = true;
                break;
            case SDLK_LEFT:
                left = true;
                break;
            case SDLK_RIGHT:
                right = true;
                break;
            default:
                break;
        }
    }

    void handleKeyUp(SDL_Keycode keyCode){
        switch (keyCode) {
            case SDLK_UP:
                up = false;
                break;
            case SDLK_DOWN:
                down = false;
                break;
            case SDLK_LEFT:
                left = false;
                break;
            case SDLK_RIGHT:
                right = false;
                break;
            default:
                break;
        }
    }

    MovementDirection getInputDirections(){
        return {
            horizontal.getAverage(),
            vertical.getAverage()
        };
    }

};
