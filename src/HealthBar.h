#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "GameObjectRenderers/UIRenderer.h"

class HealthBar: public IRenderable {

public:

    void setHealth(float health) {
        setSize(health, getHeight());
    }

    void render(RenderProps renderProps) override {
        UIRenderer::render(renderProps, this);
    };

};
