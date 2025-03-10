#pragma once

#include "../Projectile.h"
#include "../interfaces/IWeapon.h"
#include <memory>

class FireBallThrower: public IWeapon {

protected:
    SDL_Texture* texture;

    std::unique_ptr<Projectile> createProjectile(Character* originChar, Character* towardsChar){
        std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>();
        projectile->setAttack(attack);
        projectile->setPosition(
            originChar->getX(),
            originChar->getY()
        );
        projectile->setSize(0.03,0.03);
        projectile->setCollisionBox(0.03,0.03);
        projectile->setTexture(texture);
        projectile->setDirection(originChar->getMovementDirectionTowards(towardsChar).normalized());
        projectile->setVelocity(0.005f);
        return projectile;
    }

public:
    FireBallThrower() {
        //
    }

    void setProjectileTexture(SDL_Texture* newTexture){
        texture = newTexture;
    }

    std::unique_ptr<IWeapon> clone() const override {
        return std::make_unique<FireBallThrower>(*this);
    }

};