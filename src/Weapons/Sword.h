#pragma once

#include "Projectile.h"
#include "WeaponComponent.h"
#include <memory>

class Sword: public WeaponComponent {

protected:
    SDL_Texture* texture;

    std::unique_ptr<Projectile> createProjectile(Character* originChar, Character* towardsChar){
        std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>();
        projectile->setAttack(attack);
        projectile->setPosition(
            originChar->getX(),
            originChar->getY()
        );
        projectile->setSize(0.35,0.03);
        projectile->addRenderComponent(std::make_unique<SpriteRenderer>(
            texture,
            Alignment::Centered
        ));
        projectile->setCollisionComponent(std::make_unique<BoxCollider>(
            0.35,
            0.03,
            Alignment::Centered
        ));
        projectile->setDirection(originChar->getMovementDirectionTowards(towardsChar).normalized());
        projectile->setVelocity(0.0f);
        projectile->setDurationInUpdates(5);
        projectile->setMaxHits(100);
        return projectile;
    }

public:
    Sword() { }

    void setProjectileTexture(SDL_Texture* newTexture){
        texture = newTexture;
    }

    std::unique_ptr<WeaponComponent> clone() const override {
        return std::make_unique<Sword>(*this);
    }

};