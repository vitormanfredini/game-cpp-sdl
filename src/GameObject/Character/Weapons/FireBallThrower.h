#pragma once

#include "Projectile.h"
#include "WeaponComponent.h"
#include <memory>
#include "RandomGenerator.h"

class FireBallThrower: public WeaponComponent {

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
        projectile->setVelocity(0.005f);
        projectile->setDurationInUpdates(120);
        return projectile;
    }

public:
    FireBallThrower() { }

    void setProjectileTexture(SDL_Texture* newTexture){
        texture = newTexture;
    }

    std::unique_ptr<WeaponComponent> clone() const override {
        return std::make_unique<FireBallThrower>(*this);
    }

};