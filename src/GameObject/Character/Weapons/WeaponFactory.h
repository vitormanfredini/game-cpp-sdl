#pragma once

#include <memory>
#include "WeaponComponent.h"
#include "Sword.h"
#include "FireBallThrower.h"
// #include <iostream>
#include <unordered_map>
#include "Audio/AudioEngine.h"

class WeaponFactory {

public:

    enum class Id {
        Sword,
        FireBall
    };

    WeaponFactory(TextureManager* textureManager, AudioEngine* audioEngine) {
        std::unique_ptr<Sword> sword = std::make_unique<Sword>();
        sword->setProjectileTexture(textureManager->loadTexture("images/projectile.png"));
        sword->setAttack(2.0f);
        sword->setFireFrequency(100);
        sword->setFireSound(audioEngine->loadSound("audio/fu.wav"));
        prototypes[Id::Sword] = std::move(sword);

        std::unique_ptr<FireBallThrower> fireBallThrower = std::make_unique<FireBallThrower>();
        fireBallThrower->setProjectileTexture(textureManager->loadTexture("images/projectile.png"));
        fireBallThrower->setAttack(0.5f);
        fireBallThrower->setFireFrequency(20);
        fireBallThrower->setFireSound(audioEngine->loadSound("audio/pew.wav"));
        prototypes[Id::FireBall] = std::move(fireBallThrower);
    }

    std::unique_ptr<WeaponComponent> create(Id weaponId){
        return prototypes[weaponId]->clone();
    }

private:
    std::unordered_map<Id,std::unique_ptr<WeaponComponent>> prototypes;
};
