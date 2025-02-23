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
            newEnemy->setVelocity(0.006f);
            newEnemy->setAttack(0.001);
            newEnemy->setHealth(1.0f);
        }
        if(enemyType == EnemyType::Bigger){
            newEnemy->setTexture(renderer->loadTexture("images/enemy_bigger.png"));
            newEnemy->setSize(0.077f,0.077f);
            newEnemy->setVelocity(0.005f);
            newEnemy->setAttack(0.002);
            newEnemy->setHealth(3.0f);
        }
        if(enemyType == EnemyType::Boss){
            newEnemy->setTexture(renderer->loadTexture("images/enemy_boss.png"));
            newEnemy->setSize(0.1f,0.1f);
            newEnemy->setVelocity(0.004f);
            newEnemy->setAttack(0.005);
            newEnemy->setHealth(15.0f);
        }

        return newEnemy;
    }

};
