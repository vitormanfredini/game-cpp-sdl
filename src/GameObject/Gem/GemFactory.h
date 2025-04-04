#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>

#include <memory>
#include "Gem.h"
#include "GemType.h"
#include "TextureManager.h"
#include "GameObject/BoxCollider.h"
#include "GameObject/SpriteRenderer.h"

class GemFactory {

public:

    GemFactory(TextureManager* textureManager){
        prototypes[GemType::Level1] = std::make_unique<Gem>(0.3f);
        prototypes[GemType::Level1]->setSize(0.033f,0.033f);
        prototypes[GemType::Level1]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/gem1.png"),
            Alignment::BottomUpCentered
        ));
        prototypes[GemType::Level1]->setCollisionComponent(std::make_unique<BoxCollider>(
            0.0025f,0.0025f,
            Alignment::Centered
        ));

        prototypes[GemType::Level2] = std::make_unique<Gem>(0.66f);
        prototypes[GemType::Level2]->setSize(0.033f,0.033f);
        prototypes[GemType::Level2]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/gem2.png"),
            Alignment::BottomUpCentered
        ));
        prototypes[GemType::Level2]->setCollisionComponent(std::make_unique<BoxCollider>(
            0.0025f,0.0025f,
            Alignment::Centered
        ));

        prototypes[GemType::Level3] = std::make_unique<Gem>(0.132f);
        prototypes[GemType::Level3]->setSize(0.033f,0.033f);
        prototypes[GemType::Level3]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/gem3.png"),
            Alignment::BottomUpCentered
        ));
        prototypes[GemType::Level3]->setCollisionComponent(std::make_unique<BoxCollider>(
            0.0025f,0.0025f,
            Alignment::Centered
        ));
    }

    std::unique_ptr<Gem> create(GemType characterType) {
        if (prototypes.find(characterType) != prototypes.end()) {
            return prototypes[characterType]->clone();
        }

        std::cerr << "unknown GemType" << std::endl;
        return nullptr;
    }

private:
    std::unordered_map<GemType, std::unique_ptr<Gem>> prototypes;

};
