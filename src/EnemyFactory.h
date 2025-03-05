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

class EnemyFactory {

public:

    static std::unique_ptr<Character> create(Renderer* renderer, EnemyType enemyType) {
        std::unique_ptr<Character> newEnemy = std::make_unique<Character>();

        if(enemyType == EnemyType::Regular){
            newEnemy->setTexture(renderer->loadTexture("images/enemy_regular.png"));
            newEnemy->setSize(0.066f,0.066f);
            newEnemy->setVelocity(0.003f);
            newEnemy->setCollisionAttack(0.001);
            newEnemy->setCollisionBox(0.066f,0.022f);
            newEnemy->setHealth(1.0f);
            newEnemy->setWeight(0.666f);
        }
        if(enemyType == EnemyType::Bigger){
            newEnemy->setTexture(renderer->loadTexture("images/enemy_bigger.png"));
            newEnemy->setSize(0.077f,0.077f);
            newEnemy->setVelocity(0.0025f);
            newEnemy->setCollisionAttack(0.002);
            newEnemy->setCollisionBox(0.077f,0.025f);
            newEnemy->setHealth(3.0f);
            newEnemy->setWeight(0.85f);
        }
        if(enemyType == EnemyType::Boss){
            newEnemy->setTexture(renderer->loadTexture("images/enemy_boss.png"));
            newEnemy->setSize(0.1f,0.1f);
            newEnemy->setVelocity(0.002f);
            newEnemy->setCollisionAttack(0.005);
            newEnemy->setCollisionBox(0.1f,0.033f);
            newEnemy->setHealth(15.0f);
            newEnemy->setWeight(2.5f);
        }

        return newEnemy;
    }

};
