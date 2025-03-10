#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "GameObjectRenderers/UIRenderer.h"
#include <memory>
#include "Character.h"
#include "Renderer.h"
#include "Camera.h"
#include "CharacterUtils.h"
#include "EnemyType.h"
#include "TextureManager.h"

class EnemyFactory {

public:

    EnemyFactory(TextureManager* textureManager){
        prototypes[EnemyType::Regular] = std::make_unique<Character>();
        prototypes[EnemyType::Regular]->setTexture(textureManager->loadTexture("images/enemy_regular.png"));
        prototypes[EnemyType::Regular]->setSize(0.066f,0.066f);
        prototypes[EnemyType::Regular]->setVelocity(0.003f);
        prototypes[EnemyType::Regular]->setCollisionAttack(0.001);
        prototypes[EnemyType::Regular]->setCollisionBox(0.066f,0.022f);
        prototypes[EnemyType::Regular]->setHealth(1.0f);
        prototypes[EnemyType::Regular]->setWeight(0.666f);

        prototypes[EnemyType::Bigger] = std::make_unique<Character>();
        prototypes[EnemyType::Bigger]->setTexture(textureManager->loadTexture("images/enemy_bigger.png"));
        prototypes[EnemyType::Bigger]->setTexture(textureManager->loadTexture("images/enemy_bigger.png"));
        prototypes[EnemyType::Bigger]->setSize(0.077f,0.077f);
        prototypes[EnemyType::Bigger]->setVelocity(0.0025f);
        prototypes[EnemyType::Bigger]->setCollisionAttack(0.002);
        prototypes[EnemyType::Bigger]->setCollisionBox(0.077f,0.025f);
        prototypes[EnemyType::Bigger]->setHealth(3.0f);
        prototypes[EnemyType::Bigger]->setWeight(0.85f);

        prototypes[EnemyType::Boss] = std::make_unique<Character>();
        prototypes[EnemyType::Boss]->setTexture(textureManager->loadTexture("images/enemy_boss.png"));
        prototypes[EnemyType::Boss]->setSize(0.1f,0.1f);
        prototypes[EnemyType::Boss]->setVelocity(0.002f);
        prototypes[EnemyType::Boss]->setCollisionAttack(0.005);
        prototypes[EnemyType::Boss]->setCollisionBox(0.1f,0.033f);
        prototypes[EnemyType::Boss]->setHealth(15.0f);
        prototypes[EnemyType::Boss]->setWeight(2.5f);
    }

    std::unique_ptr<Character> create(EnemyType enemyType) {
        if (prototypes.find(enemyType) != prototypes.end()) {
            return prototypes[enemyType]->clone();
        }

        std::cerr << "unknown EnemyType" << std::endl;
        return nullptr;
    }

private:
    std::unordered_map<EnemyType, std::unique_ptr<Character>> prototypes;

};
