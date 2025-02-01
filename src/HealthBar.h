#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"

class HealthBar: public IRenderable {
private:
    float maxWidth = 1.0f;

public:

    void setHealth(float health) {
        setSize(health * maxWidth, getHeight());
    }

    void setMaxWidth(float _maxWidth){
        maxWidth = _maxWidth;
    }

};
