#pragma once

#include "RenderProps.h"
#include "GameObjectRenderers/GameWorldRenderer.h"

class StaticSprite: public IRenderable {

public:

    void render(RenderProps renderProps) override {
        GameWorldRenderer::render(renderProps, this);
    };

};
