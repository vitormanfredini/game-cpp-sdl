#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"

#include <memory>
#include "Character.h"
#include "Renderer.h"
#include "Camera.h"
#include "CharacterUtils.h"
#include "CharacterType.h"
#include "TextureManager.h"
#include "GameObject/BoxCollider.h"
#include "GameObject/CharacterHealthBarRenderer.h"

class CharacterFactory {

public:

CharacterFactory(TextureManager* textureManager){
        prototypes[CharacterType::Regular] = std::make_unique<Character>();
        prototypes[CharacterType::Regular]->setSize(0.066f,0.066f);
        prototypes[CharacterType::Regular]->setVelocity(0.003f);
        prototypes[CharacterType::Regular]->setCollisionAttack(0.001);
        prototypes[CharacterType::Regular]->setInitialHealth(1.0f);
        prototypes[CharacterType::Regular]->setWeight(0.666f);
        prototypes[CharacterType::Regular]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/enemy_regular.png"),
            Alignment::BottomUpCentered
        ));
        prototypes[CharacterType::Regular]->addRenderComponent(std::make_unique<CharacterHealthBarRenderer>(
            textureManager->loadTexture(87, 45, 53),
            textureManager->loadTexture(200,69,49),
            &prototypes[CharacterType::Regular]->getHealthPercentage()
        ));

        prototypes[CharacterType::Regular]->setCollisionComponent(std::make_unique<BoxCollider>(
            0.066f,
            0.022f,
            Alignment::Centered
        ));

        prototypes[CharacterType::Bigger] = std::make_unique<Character>();
        prototypes[CharacterType::Bigger]->setSize(0.077f,0.077f);
        prototypes[CharacterType::Bigger]->setVelocity(0.0025f);
        prototypes[CharacterType::Bigger]->setCollisionAttack(0.002);
        prototypes[CharacterType::Bigger]->setInitialHealth(3.0f);
        prototypes[CharacterType::Bigger]->setWeight(0.85f);
        prototypes[CharacterType::Bigger]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/enemy_bigger.png"),
            Alignment::BottomUpCentered
        ));
        prototypes[CharacterType::Bigger]->addRenderComponent(std::make_unique<CharacterHealthBarRenderer>(
            textureManager->loadTexture(87, 45, 53),
            textureManager->loadTexture(200,69,49),
            &prototypes[CharacterType::Bigger]->getHealthPercentage()
        ));
        prototypes[CharacterType::Bigger]->setCollisionComponent(std::make_unique<BoxCollider>(
            0.077f,
            0.025f,
            Alignment::Centered
        ));

        prototypes[CharacterType::Boss] = std::make_unique<Character>();
        prototypes[CharacterType::Boss]->setSize(0.1f,0.1f);
        prototypes[CharacterType::Boss]->setVelocity(0.002f);
        prototypes[CharacterType::Boss]->setCollisionAttack(0.005);
        prototypes[CharacterType::Boss]->setInitialHealth(15.0f);
        prototypes[CharacterType::Boss]->setWeight(2.5f);
        prototypes[CharacterType::Boss]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/enemy_boss.png"),
            Alignment::BottomUpCentered
        ));
        prototypes[CharacterType::Boss]->addRenderComponent(std::make_unique<CharacterHealthBarRenderer>(
            textureManager->loadTexture(87, 45, 53),
            textureManager->loadTexture(200,69,49),
            &prototypes[CharacterType::Boss]->getHealthPercentage()
        ));
        prototypes[CharacterType::Boss]->setCollisionComponent(std::make_unique<BoxCollider>(
            0.1f,
            0.033f,
            Alignment::Centered
        ));
    }

    std::unique_ptr<Character> create(CharacterType characterType) {
        if (prototypes.find(characterType) != prototypes.end()) {
            return prototypes[characterType]->clone();
        }

        std::cerr << "unknown CharacterType" << std::endl;
        return nullptr;
    }

private:
    std::unordered_map<CharacterType, std::unique_ptr<Character>> prototypes;

};
