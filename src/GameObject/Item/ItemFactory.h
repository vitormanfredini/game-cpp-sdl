#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>

#include <memory>
#include "Item.h"
#include "ItemId.h"
#include "TextureManager.h"
#include "GameObject/BoxCollider.h"
#include "GameObject/SpriteRenderer.h"
#include "GameObject/Movement/MagneticMover.h"

class ItemFactory {

public:

    ItemFactory(TextureManager* textureManager){

        std::unique_ptr<ShadowRenderer> defaultItemShadow = std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.01
        );

        prototypes[ItemId::Gem] = std::make_unique<Item>(ItemId::Gem, 0.3f);
        prototypes[ItemId::Gem]->addRenderComponent(defaultItemShadow->clone());
        prototypes[ItemId::Gem]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/item_gem1.png"),
            Alignment::BottomUpCentered
        ));
        

        prototypes[ItemId::Health] = std::make_unique<Item>(ItemId::Health, 0.1f);
        prototypes[ItemId::Health]->addRenderComponent(defaultItemShadow->clone());
        prototypes[ItemId::Health]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/item_health.png"),
            Alignment::BottomUpCentered
        ));
    }

    std::unique_ptr<Item> create(ItemId itemId) {
        if (prototypes.find(itemId) != prototypes.end()) {
            std::unique_ptr<Item> copy = prototypes[itemId]->clone();
            copy->setSize(0.033f,0.033f);
            copy->setCollisionComponent(std::make_unique<BoxCollider>(
                0.0025f,0.0025f,
                Alignment::BottomUpCentered
            ));
            copy->setMovementComponent(std::make_unique<MagneticMover>());
            copy->setZ(0.015f);
            return copy;
        }

        std::cerr << "unknown ItemId" << std::endl;
        return nullptr;
    }

private:
    std::unordered_map<ItemId, std::unique_ptr<Item>> prototypes;

};
