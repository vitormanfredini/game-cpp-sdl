#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "interfaces/ICollidable.h"
#include "interfaces/IRenderable.h"
#include "Projectile.h"
#include "GameObjectRenderers/GameWorldRenderer.h"
#include <memory>

class Character: public ICollidable, public IRenderable {

private:
    float velocity = 0.01f;
    float health = 1.0f;
    float attack = 0.001;

    int fireEveryXUpdates = 15;
    int updatesUntilNextFire = 0;

public:

    void update(){
        if(updatesUntilNextFire <= 0){
            updatesUntilNextFire = fireEveryXUpdates;
        }
        updatesUntilNextFire -= 1;
    }

    float getHealth(){
        return health;
    }

    bool isDead(){
        return health <= 0.0f;
    }

    float getAttack(){
        return attack;
    }

    void setVelocity(float newVelocity) {
        velocity = newVelocity;
    }

    void move(MovementDirection directions){
        y += directions.vertical * velocity;
        x += directions.horizontal * velocity;
    }

    MovementDirection getMovementDirectionTowards(Character* other){
        float deltaX = other->getX() + (other->getWidth() / 2) - (x + (getWidth() / 2));
        float absDeltaX = abs(deltaX);
        float deltaY = other->getY() + (other->getHeight() / 2) - (y + (getHeight() / 2));
        float absDeltaY = abs(deltaY);

        float maxAbsDelta = absDeltaX > absDeltaY ? absDeltaX : absDeltaY;

        return MovementDirection {
            deltaX / maxAbsDelta,
            deltaY / maxAbsDelta,
        };
    }

    void moveTowards(Character* other){
        move(getMovementDirectionTowards(other));
    }

    void takeDamageFrom(Projectile* projectile){
        if(projectile->isDead()){
            return;
        }
        health -= projectile->getAttack();
        projectile->takeHit();
    }

    void takeDamageFrom(Character* other){
        if(other->isDead()){
            return;
        }
        health -= other->getAttack();
    }

    double distanceFrom(Character* other){
        return sqrt(pow(other->getX() - x, 2) +  pow(y - other->getY(), 2));
    }

    bool shouldFireProjectile(){
        return updatesUntilNextFire == 0;
    }

    void render(RenderProps renderProps) override {
        GameWorldRenderer::render(renderProps, this);
    };

    std::unique_ptr<Projectile> createProjectile(Character* towardsOther, SDL_Texture* texture_projectile){
        std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>();
        projectile->setAttack(1.0f);
        projectile->setPosition(
            getX() + (getWidth() / 2.0f),
            getY() + (getHeight() / 2.0f)
        );
        projectile->setSize(0.03,0.03);
        projectile->setTexture(texture_projectile);
        projectile->setDirection(getMovementDirectionTowards(towardsOther));
        projectile->setVelocity(0.01f);
        return projectile;
    }

};
