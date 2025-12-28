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
#include "GameObject/Movement/Floating/SineFloater.h"
#include "GameObject/ShadowRenderer.h"
#include "SineLookupTable.h"

class CharacterFactory {

public:

    CharacterFactory(TextureManager* textureManager){

        std::unique_ptr<CharacterHealthBarRenderer> defaultHealthBarRenderer = std::make_unique<CharacterHealthBarRenderer>(
            textureManager->loadTexture(87, 45, 53),
            textureManager->loadTexture(200,69,49)
        );

        float heightEnemy1 = 0.09;
        float widthRatioEnemy1 = 0.4615;
        prototypes[CharacterType::Regular] = std::make_unique<Character>();
        prototypes[CharacterType::Regular]->setSize(heightEnemy1*widthRatioEnemy1,heightEnemy1);
        prototypes[CharacterType::Regular]->setInitialBaseSpeed(0.4f);
        prototypes[CharacterType::Regular]->setCollisionAttack(0.005);
        prototypes[CharacterType::Regular]->setInitialMaxHealth(1.0f);
        prototypes[CharacterType::Regular]->setInitialHealth(3.0f);
        prototypes[CharacterType::Regular]->setWeight(0.666f);
        prototypes[CharacterType::Regular]->setZ(heightEnemy1 * 0.3);
        prototypes[CharacterType::Regular]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.017
        ));
        SDL_Texture* enemy1Texture = nullptr;
        SDL_Texture* enemy1TextureBeenHit = nullptr;
        textureManager->loadTextureAndBeenHitTexture("images/chars/enemy2_horizontal_sprites.png", enemy1Texture, enemy1TextureBeenHit);
        prototypes[CharacterType::Regular]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            enemy1Texture,
            enemy1TextureBeenHit,
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
        prototypes[CharacterType::Regular]->setFloatingComponent(std::make_unique<SineFloater<100>>(SineLookupTable<100>::table, 0.01f));
        prototypes[CharacterType::Regular]->addDebrisType(DebrisFactory::Type::MetalSmall);

        float heightEnemy2 = 0.07;
        float widthRatioEnemy2 = 1.1562;
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
        SDL_Texture* enemy2Texture = nullptr;
        SDL_Texture* enemy2TextureBeenHit = nullptr;
        textureManager->loadTextureAndBeenHitTexture("images/chars/enemy1_horizontal_sprites.png", enemy2Texture, enemy2TextureBeenHit);
        prototypes[CharacterType::Bigger]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            enemy2Texture,
            enemy2TextureBeenHit,
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
        prototypes[CharacterType::Bigger]->setFloatingComponent(std::make_unique<SineFloater<140>>(SineLookupTable<140>::table, 0.01f));
        prototypes[CharacterType::Bigger]->addDebrisType(DebrisFactory::Type::MetalSmall);
        prototypes[CharacterType::Bigger]->addDebrisType(DebrisFactory::Type::MetalSmall2);
        prototypes[CharacterType::Bigger]->addDebrisType(DebrisFactory::Type::Oil);

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
        SDL_Texture* enemy3Texture = nullptr;
        SDL_Texture* enemy3TextureBeenHit = nullptr;
        textureManager->loadTextureAndBeenHitTexture("images/chars/enemy3_horizontal_sprites.png", enemy3Texture, enemy3TextureBeenHit);
        prototypes[CharacterType::Boss]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            enemy3Texture,
            enemy3TextureBeenHit,
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
        prototypes[CharacterType::Boss]->addDebrisType(DebrisFactory::Type::MetalSmall);
        prototypes[CharacterType::Boss]->addDebrisType(DebrisFactory::Type::MetalSmall2);
        prototypes[CharacterType::Boss]->addDebrisType(DebrisFactory::Type::MetalBar);
        prototypes[CharacterType::Boss]->addDebrisType(DebrisFactory::Type::Oil);
        prototypes[CharacterType::Boss]->addDebrisType(DebrisFactory::Type::Oil2);

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
        SDL_Texture* enemy4Texture = nullptr;
        SDL_Texture* enemy4TextureBeenHit = nullptr;
        textureManager->loadTextureAndBeenHitTexture("images/chars/enemy4_horizontal_sprites.png", enemy4Texture, enemy4TextureBeenHit);
        prototypes[CharacterType::Fire]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            enemy4Texture,
            enemy4TextureBeenHit,
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
        prototypes[CharacterType::Fire]->addDebrisType(DebrisFactory::Type::Oil);
        prototypes[CharacterType::Fire]->addDebrisType(DebrisFactory::Type::Oil2);

        float heightEnemy5 = 0.11;
        float widthRatioEnemy5 = 0.4;
        prototypes[CharacterType::Furnace] = std::make_unique<Character>();
        prototypes[CharacterType::Furnace]->setSize(heightEnemy5*widthRatioEnemy5,heightEnemy5);
        prototypes[CharacterType::Furnace]->setInitialBaseSpeed(0.6f);
        prototypes[CharacterType::Furnace]->setCollisionAttack(0.0035);
        prototypes[CharacterType::Furnace]->setInitialMaxHealth(2.0f);
        prototypes[CharacterType::Furnace]->setInitialHealth(2.0f);
        prototypes[CharacterType::Furnace]->setWeight(0.5f);
        prototypes[CharacterType::Furnace]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.025
        ));
        SDL_Texture* enemy5Texture = nullptr;
        SDL_Texture* enemy5TextureBeenHit = nullptr;
        textureManager->loadTextureAndBeenHitTexture("images/chars/enemy5_horizontal_sprites.png", enemy5Texture, enemy5TextureBeenHit);
        prototypes[CharacterType::Furnace]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            enemy5Texture,
            enemy5TextureBeenHit,
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
        prototypes[CharacterType::Furnace]->addDebrisType(DebrisFactory::Type::MetalSmall2);

        float heightEnemy6 = 0.17;
        float widthRatioEnemy6 = 0.56;
        prototypes[CharacterType::DataCenterRack] = std::make_unique<Character>();
        prototypes[CharacterType::DataCenterRack]->setSize(heightEnemy6*widthRatioEnemy6,heightEnemy6);
        prototypes[CharacterType::DataCenterRack]->setInitialBaseSpeed(0.3f);
        prototypes[CharacterType::DataCenterRack]->setCollisionAttack(0.008);
        prototypes[CharacterType::DataCenterRack]->setInitialMaxHealth(6.0f);
        prototypes[CharacterType::DataCenterRack]->setInitialHealth(6.0f);
        prototypes[CharacterType::DataCenterRack]->setWeight(1.5f);
        prototypes[CharacterType::DataCenterRack]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.045
        ));
        SDL_Texture* enemy6Texture = nullptr;
        SDL_Texture* enemy6TextureBeenHit = nullptr;
        textureManager->loadTextureAndBeenHitTexture("images/chars/enemy6_horizontal_sprites.png", enemy6Texture, enemy6TextureBeenHit);
        prototypes[CharacterType::DataCenterRack]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            enemy6Texture,
            enemy6TextureBeenHit,
            Alignment::BottomUpCentered,
            5,
            37
        ));
        prototypes[CharacterType::DataCenterRack]->addRenderComponent(defaultHealthBarRenderer->clone());
        prototypes[CharacterType::DataCenterRack]->setCollisionComponent(std::make_unique<BoxCollider>(
            heightEnemy6*widthRatioEnemy6,
            heightEnemy6 / 3,
            Alignment::Centered
        ));
        prototypes[CharacterType::DataCenterRack]->setMovementComponent(std::make_unique<StraightMover>());
        prototypes[CharacterType::DataCenterRack]->addDebrisType(DebrisFactory::Type::MetalSmall);
        prototypes[CharacterType::DataCenterRack]->addDebrisType(DebrisFactory::Type::MetalSmall2);

        float heightEnemy7 = 0.055;
        float widthRatioEnemy7 = 1.4545;
        prototypes[CharacterType::NeuralNet] = std::make_unique<Character>();
        prototypes[CharacterType::NeuralNet]->setSize(heightEnemy7*widthRatioEnemy7,heightEnemy7);
        prototypes[CharacterType::NeuralNet]->setZ(heightEnemy7 * 0.2);
        prototypes[CharacterType::NeuralNet]->setInitialBaseSpeed(0.5f);
        prototypes[CharacterType::NeuralNet]->setCollisionAttack(0.02);
        prototypes[CharacterType::NeuralNet]->setInitialMaxHealth(0.6f);
        prototypes[CharacterType::NeuralNet]->setInitialHealth(0.6f);
        prototypes[CharacterType::NeuralNet]->setWeight(0.66f);
        prototypes[CharacterType::NeuralNet]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.03
        ));
        SDL_Texture* enemy7Texture = nullptr;
        SDL_Texture* enemy7TextureBeenHit = nullptr;
        textureManager->loadTextureAndBeenHitTexture("images/chars/enemy7_horizontal_sprites.png", enemy7Texture, enemy7TextureBeenHit);
        prototypes[CharacterType::NeuralNet]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            enemy7Texture,
            enemy7TextureBeenHit,
            Alignment::BottomUpCentered,
            3,
            48
        ));
        prototypes[CharacterType::NeuralNet]->addRenderComponent(defaultHealthBarRenderer->clone());
        prototypes[CharacterType::NeuralNet]->setCollisionComponent(std::make_unique<BoxCollider>(
            heightEnemy7*widthRatioEnemy7,
            heightEnemy7 / 3,
            Alignment::Centered
        ));
        prototypes[CharacterType::NeuralNet]->setMovementComponent(std::make_unique<StraightMover>());
        prototypes[CharacterType::NeuralNet]->addDebrisType(DebrisFactory::Type::MetalSmall);
        prototypes[CharacterType::NeuralNet]->addDebrisType(DebrisFactory::Type::MetalSmall2);
        prototypes[CharacterType::NeuralNet]->addDebrisType(DebrisFactory::Type::Oil);
        prototypes[CharacterType::NeuralNet]->addDebrisType(DebrisFactory::Type::Oil2);

        float heightEnemy8 = 0.1;
        float widthRatioEnemy8 = 1.4545;
        prototypes[CharacterType::FinalBoss] = std::make_unique<Character>();
        prototypes[CharacterType::FinalBoss]->setSize(heightEnemy8*widthRatioEnemy8,heightEnemy8);
        prototypes[CharacterType::FinalBoss]->setZ(heightEnemy8 * 0.2);
        prototypes[CharacterType::FinalBoss]->setInitialBaseSpeed(0.5f);
        prototypes[CharacterType::FinalBoss]->setCollisionAttack(0.02);
        prototypes[CharacterType::FinalBoss]->setInitialMaxHealth(0.6f);
        prototypes[CharacterType::FinalBoss]->setInitialHealth(0.6f);
        prototypes[CharacterType::FinalBoss]->setWeight(0.66f);
        prototypes[CharacterType::FinalBoss]->addRenderComponent(std::make_unique<ShadowRenderer>(
            textureManager->loadTexture("images/chars/shadow.png"),
            0.03
        ));
        SDL_Texture* enemy8Texture = nullptr;
        SDL_Texture* enemy8TextureBeenHit = nullptr;
        textureManager->loadTextureAndBeenHitTexture("images/chars/enemy7_horizontal_sprites.png", enemy8Texture, enemy8TextureBeenHit);
        prototypes[CharacterType::FinalBoss]->addRenderComponent(std::make_unique<HorizontalSpriteAnimationRenderer>(
            enemy8Texture,
            enemy8TextureBeenHit,
            Alignment::BottomUpCentered,
            3,
            48
        ));
        prototypes[CharacterType::FinalBoss]->addRenderComponent(defaultHealthBarRenderer->clone());
        prototypes[CharacterType::FinalBoss]->setCollisionComponent(std::make_unique<BoxCollider>(
            heightEnemy8*widthRatioEnemy8,
            heightEnemy8 / 3,
            Alignment::Centered
        ));
        prototypes[CharacterType::FinalBoss]->setMovementComponent(std::make_unique<StraightMover>());
        prototypes[CharacterType::FinalBoss]->addDebrisType(DebrisFactory::Type::MetalSmall);
        prototypes[CharacterType::FinalBoss]->addDebrisType(DebrisFactory::Type::MetalSmall2);
        prototypes[CharacterType::FinalBoss]->addDebrisType(DebrisFactory::Type::Oil);
        prototypes[CharacterType::FinalBoss]->addDebrisType(DebrisFactory::Type::Oil2);

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
