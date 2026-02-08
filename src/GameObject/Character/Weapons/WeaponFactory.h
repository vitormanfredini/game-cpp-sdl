#pragma once

#include <memory>
#include "WeaponComponent.h"
#include "Sword.h"
#include "WeaponId.h"
#include "FireBallThrower.h"
#include "BossGun.h"
// #include <iostream>
#include <unordered_map>
#include "Audio/AudioEngine.h"

class WeaponFactory {

public:

    WeaponFactory(TextureManager* textureManager, AudioEngine* audioEngine) {
        std::unique_ptr<Sword> sword = std::make_unique<Sword>();
        sword->setId(WeaponId::Sword);
        sword->setProjectileTexture(textureManager->loadTexture("images/projectile.png"));
        sword->setAttack(2.0f);
        sword->setFireFrequency(100);
        sword->setFireSound(audioEngine->loadSound("audio/fu.wav"));
        prototypes[WeaponId::Sword] = std::move(sword);

        std::unique_ptr<FireBallThrower> fireBallThrower = std::make_unique<FireBallThrower>();
        fireBallThrower->setId(WeaponId::FireBall);
        fireBallThrower->setProjectileTexture(textureManager->loadTexture("images/projectile.png"));
        fireBallThrower->setAttack(0.5f);
        fireBallThrower->setFireFrequency(20);
        fireBallThrower->setFireSound(audioEngine->loadSound("audio/pew.wav"));
        prototypes[WeaponId::FireBall] = std::move(fireBallThrower);

        std::unique_ptr<BossGun> bossGun = std::make_unique<BossGun>();
        bossGun->setId(WeaponId::BossGun);
        bossGun->setProjectileTexture(textureManager->loadTexture("images/dog.png"));
        bossGun->setAttack(0.5f);
        bossGun->setFireFrequency(20);
        bossGun->setFireSound(audioEngine->loadSound("audio/pew.wav"));
        prototypes[WeaponId::FireBall] = std::move(bossGun);

    }

    std::unique_ptr<WeaponComponent> create(WeaponId weaponId){
        return prototypes[weaponId]->clone();
    }

private:
    std::unordered_map<WeaponId,std::unique_ptr<WeaponComponent>> prototypes;
};
