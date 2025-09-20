#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>

#include <memory>
#include "Character.h"
#include "Renderer.h"
#include "Camera.h"
#include "CharacterUtils.h"
#include "CharacterType.h"
#include "TextureManager.h"
#include "GameObject/BoxCollider.h"
#include "GameObject/CharacterHealthBarRenderer.h"
#include "GameObject/SpriteRenderer.h"
#include "GameObject/HorizontalSpriteAnimationRenderer.h"
#include "GameObject/Movement/StraightMover.h"
#include "GameObject/Movement/AngledMover.h"
#include "GameObject/Movement/ZigZagMover.h"
#include "GameObject/ShadowRenderer.h"

class CharacterFactory {

public:

    CharacterFactory(TextureManager* textureManager){

        std::unique_ptr<CharacterHealthBarRenderer> defaultHealthBarRenderer = std::make_unique<CharacterHealthBarRenderer>(
            textureManager->loadTexture(87, 45, 53),
            textureManager->loadTexture(200,69,49)
        );

        float heightEnemy1 = 0.09;
        float widthRatioEnemy1 = 0.42;
        prototypes[CharacterType::Regular] = std::make_unique<Character>();
        prototypes[CharacterType::Regular]->setSize(heightEnemy1*widthRatioEnemy1,heightEnemy1);
        prototypes[CharacterType::Regular]->setInitialBaseSpeed(0.4f);
        prototypes[CharacterType::Regular]->setCollisionAttack(0.005);
        prototypes[CharacterType::Regular]->setInitialMaxHealth(1.0f);
        prototypes[CharacterType::Regular]->setInitialHealth(3.0f);
        prototypes[CharacterType::Regular]->setWeight(0.666f);
        prototypes[CharacterType::Regular]->setZ(heightEnemy1 * 0.25);
        prototypes[CharacterType::Regular]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.017
        ));
        prototypes[CharacterType::Regular]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            textureManager->loadTexture("images/chars/enemy2_horizontal_sprites.png"),
            Alignment::BottomUpCentered,
            5,
            18
        ));
        prototypes[CharacterType::Regular]->addRenderComponent(defaultHealthBarRenderer->clone());
        prototypes[CharacterType::Regular]->setCollisionComponent(std::make_unique<BoxCollider>(
            heightEnemy1*widthRatioEnemy1,
            heightEnemy1 / 3,
            Alignment::Centered
        ));
        prototypes[CharacterType::Regular]->setMovementComponent(std::make_unique<ZigZagMover>(45, 0.02));

        float heightEnemy2 = 0.07;
        float widthRatioEnemy2 = 1.03;
        prototypes[CharacterType::Bigger] = std::make_unique<Character>();
        prototypes[CharacterType::Bigger]->setSize(heightEnemy2*widthRatioEnemy2,heightEnemy2);
        prototypes[CharacterType::Bigger]->setInitialBaseSpeed(0.33f);
        prototypes[CharacterType::Bigger]->setCollisionAttack(0.008);
        prototypes[CharacterType::Bigger]->setInitialMaxHealth(3.0f);
        prototypes[CharacterType::Bigger]->setInitialHealth(3.0f);
        prototypes[CharacterType::Bigger]->setWeight(0.85f);
        prototypes[CharacterType::Bigger]->setZ(heightEnemy2 * 0.25);
        prototypes[CharacterType::Bigger]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.02
        ));
        prototypes[CharacterType::Bigger]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            textureManager->loadTexture("images/chars/enemy1_horizontal_sprites.png"),
            Alignment::BottomUpCentered,
            5,
            37
        ));
        prototypes[CharacterType::Bigger]->addRenderComponent(defaultHealthBarRenderer->clone());
        prototypes[CharacterType::Bigger]->setCollisionComponent(std::make_unique<BoxCollider>(
            heightEnemy2*widthRatioEnemy2,
            heightEnemy2 / 3,
            Alignment::Centered
        ));
        prototypes[CharacterType::Bigger]->setMovementComponent(std::make_unique<AngledMover>(45, 0.12, 0.25, 0.33, 0.5));

        float heightEnemy3 = 0.14;
        float widthRatioEnemy3 = 0.66;
        prototypes[CharacterType::Boss] = std::make_unique<Character>();
        prototypes[CharacterType::Boss]->setSize(heightEnemy3*widthRatioEnemy3,heightEnemy3);
        prototypes[CharacterType::Boss]->setInitialBaseSpeed(0.2f);
        prototypes[CharacterType::Boss]->setCollisionAttack(0.015);
        prototypes[CharacterType::Boss]->setInitialMaxHealth(15.0f);
        prototypes[CharacterType::Boss]->setInitialHealth(15.0f);
        prototypes[CharacterType::Boss]->setWeight(2.5f);
        prototypes[CharacterType::Boss]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.025
        ));
        prototypes[CharacterType::Boss]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            textureManager->loadTexture("images/chars/enemy3_horizontal_sprites.png"),
            Alignment::BottomUpCentered,
            8,
            34
        ));
        prototypes[CharacterType::Boss]->addRenderComponent(defaultHealthBarRenderer->clone());
        prototypes[CharacterType::Boss]->setCollisionComponent(std::make_unique<BoxCollider>(
            heightEnemy3*widthRatioEnemy3,
            heightEnemy3 / 3,
            Alignment::Centered
        ));
        prototypes[CharacterType::Boss]->setMovementComponent(std::make_unique<StraightMover>());

        float heightEnemy4 = 0.2;
        float widthRatioEnemy4 = 0.325;
        prototypes[CharacterType::Fire] = std::make_unique<Character>();
        prototypes[CharacterType::Fire]->setSize(heightEnemy4*widthRatioEnemy4,heightEnemy4);
        prototypes[CharacterType::Fire]->setInitialBaseSpeed(0.3f);
        prototypes[CharacterType::Fire]->setCollisionAttack(0.015);
        prototypes[CharacterType::Fire]->setInitialMaxHealth(11.0f);
        prototypes[CharacterType::Fire]->setInitialHealth(11.0f);
        prototypes[CharacterType::Fire]->setWeight(0.8f);
        prototypes[CharacterType::Fire]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.025
        ));
        prototypes[CharacterType::Fire]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            textureManager->loadTexture("images/chars/enemy4_horizontal_sprites.png"),
            Alignment::BottomUpCentered,
            6,
            28
        ));
        prototypes[CharacterType::Fire]->addRenderComponent(defaultHealthBarRenderer->clone());
        prototypes[CharacterType::Fire]->setCollisionComponent(std::make_unique<BoxCollider>(
            heightEnemy4*widthRatioEnemy4,
            heightEnemy4 / 3,
            Alignment::Centered
        ));
        prototypes[CharacterType::Fire]->setMovementComponent(std::make_unique<StraightMover>());

        float heightEnemy5 = 0.11;
        float widthRatioEnemy5 = 0.4;
        prototypes[CharacterType::Furnace] = std::make_unique<Character>();
        prototypes[CharacterType::Furnace]->setSize(heightEnemy5*widthRatioEnemy5,heightEnemy5);
        prototypes[CharacterType::Furnace]->setInitialBaseSpeed(0.6f);
        prototypes[CharacterType::Furnace]->setCollisionAttack(0.0035);
        prototypes[CharacterType::Furnace]->setInitialMaxHealth(1.0f);
        prototypes[CharacterType::Furnace]->setInitialHealth(2.0f);
        prototypes[CharacterType::Furnace]->setWeight(0.5f);
        prototypes[CharacterType::Furnace]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.025
        ));
        prototypes[CharacterType::Furnace]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            textureManager->loadTexture("images/chars/enemy5_horizontal_sprites.png"),
            Alignment::BottomUpCentered,
            6,
            24
        ));
        prototypes[CharacterType::Furnace]->addRenderComponent(defaultHealthBarRenderer->clone());
        prototypes[CharacterType::Furnace]->setCollisionComponent(std::make_unique<BoxCollider>(
            heightEnemy5*widthRatioEnemy5,
            heightEnemy5 / 3,
            Alignment::Centered
        ));
        prototypes[CharacterType::Furnace]->setMovementComponent(std::make_unique<ZigZagMover>(60, 0.01));
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
