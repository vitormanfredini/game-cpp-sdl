#pragma once

#include <SDL2/SDL_render.h>
#include "WeaponComponent.h"
#include <memory>
#include "RandomGenerator.h"
#include "Projectile.h"

class BossGun: public WeaponComponent {

protected:
    SDL_Texture* texture;

    std::unique_ptr<Projectile> createProjectile(Character* originChar){
        std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>();
        projectile->setAttack(attack);
        projectile->setPosition(
            originChar->getX(),
            originChar->getY()
        );
        projectile->setZ(originChar->getHeight() / 2);
        projectile->setSize(0.03,0.03);
        projectile->addRenderComponent(std::make_unique<SpriteRenderer>(
            texture,
            Alignment::Centered
        ));
        projectile->setCollisionComponent(std::make_unique<BoxCollider>(
            0.03,
            0.03,
            Alignment::Centered
        ));
        projectile->setVelocity(0.015f);
        projectile->setDurationInUpdates(240);
        return projectile;
    }

public:
    BossGun() { }

    void setProjectileTexture(SDL_Texture* newTexture){
        texture = newTexture;
    }

    std::unique_ptr<WeaponComponent> clone() const override {
        return std::make_unique<BossGun>(*this);
    }

};