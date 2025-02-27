#pragma once

#include "../Projectile.h"
#include "../interfaces/IWeapon.h"
#include <memory>

class FireBallThrower: public IWeapon {

protected:
    SDL_Texture* texture;

    std::unique_ptr<Projectile> createProjectile(Character* originChar, Character* towardsChar){
        std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>();
        projectile->setAttack(0.3f);
        projectile->setPosition(
            originChar->getX() + (originChar->getWidth() / 2.0f),
            originChar->getY() + (originChar->getHeight() / 2.0f)
        );
        projectile->setSize(0.03,0.03);
        projectile->setTexture(texture);
        projectile->setDirection(originChar->getMovementDirectionTowards(towardsChar));
        projectile->setVelocity(0.01f);
        return projectile;
    }

public:
    FireBallThrower(int updatesBetweenFires, SDL_Texture* projectileTexture) : IWeapon(updatesBetweenFires), texture(projectileTexture) {
        //
    }

};