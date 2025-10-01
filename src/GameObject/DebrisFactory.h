#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include <memory>
#include "TextureManager.h"
#include "GameObject/SpriteRenderer.h"

class DebrisFactory {

public:

    DebrisFactory(TextureManager* textureManager){

        prototype = std::make_unique<GameObject>();
        float size = 0.025;
        prototype->setSize(size, size);
        prototype->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/chars/debris.png"),
            Alignment::Centered
        ));
    }

    std::unique_ptr<GameObject> create() {
        std::unique_ptr<GameObject> copy = prototype->clone();
        return copy;
    }

private:
    std::unique_ptr<GameObject> prototype;

};
