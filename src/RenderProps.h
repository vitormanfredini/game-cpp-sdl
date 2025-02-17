#pragma once
#include <SDL2/SDL_render.h>

struct RenderProps {
    SDL_Renderer* sdl_renderer;
    
    float cameraPosX;
    float cameraPosY;

    float topOffset;
    float leftOffset;
    
    float screenScale;
};