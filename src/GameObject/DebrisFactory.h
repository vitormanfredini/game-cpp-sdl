#pragma once

#include <string.h>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "resource_registry.h"
#include "TextureManager.h"
#include "GameObject/SpriteRenderer.h"

class DebrisFactory {

public:

    enum class Type {
        MetalSmall,
        MetalSmall2,
        Oil,
        Oil2,
        MetalBar,
    };

    DebrisFactory(TextureManager* textureManager){

        prototypes[DebrisFactory::Type::MetalSmall] = std::make_unique<GameObject>();
        prototypes[DebrisFactory::Type::MetalSmall]->setSize(0.025, 0.025);
        prototypes[DebrisFactory::Type::MetalSmall]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/chars/debris_metal_small.png"),
            Alignment::Centered
        ));

        prototypes[DebrisFactory::Type::MetalSmall2] = std::make_unique<GameObject>();
        prototypes[DebrisFactory::Type::MetalSmall2]->setSize(0.028, 0.028);
        prototypes[DebrisFactory::Type::MetalSmall2]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/chars/debris_metal_small2.png"),
            Alignment::Centered
        ));

        prototypes[DebrisFactory::Type::Oil] = std::make_unique<GameObject>();
        prototypes[DebrisFactory::Type::Oil]->setSize(0.02, 0.02);
        prototypes[DebrisFactory::Type::Oil]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/chars/debris_oil.png"),
            Alignment::Centered
        ));

        prototypes[DebrisFactory::Type::Oil2] = std::make_unique<GameObject>();
        prototypes[DebrisFactory::Type::Oil2]->setSize(0.025, 0.025);
        prototypes[DebrisFactory::Type::Oil2]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/chars/debris_oil.png"),
            Alignment::Centered
        ));
    }

    std::unique_ptr<GameObject> create(DebrisFactory::Type type) {
        return prototypes[type]->clone();
    }

private:
    std::unordered_map<DebrisFactory::Type, std::unique_ptr<GameObject>> prototypes;

};
