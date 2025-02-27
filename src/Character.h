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
#include "interfaces/IWeapon.h"

class Character: public ICollidable, public IRenderable {

private:
    float velocity = 0.01f;
    float health = 1.0f;
    float collisionAttack = 0.001;

    std::vector<std::unique_ptr<IWeapon>> weapons = {};

public:

    void addWeapon(std::unique_ptr<IWeapon> weapon){
        weapons.push_back(std::move(weapon));
    }

    void update(){
        for(std::unique_ptr<IWeapon>& weapon : weapons){
            weapon->update();
        }
    }

    std::vector<std::unique_ptr<Projectile>> fire(Character* towardsChar){
        std::vector<std::unique_ptr<Projectile>> newProjectiles = {};
        for(std::unique_ptr<IWeapon>& weapon : weapons){
            for(std::unique_ptr<Projectile>& projectile : weapon->fire(this, towardsChar)){
                newProjectiles.push_back(std::move(projectile));
            }
        }
        return newProjectiles;
    }

    float getHealth(){
        return health;
    }

    void setHealth(float newHealth){
        health = newHealth;
    }

    bool isDead(){
        return health <= 0.0f;
    }

    float getCollisionAttack(){
        return collisionAttack;
    }

    void setCollisionAttack(float newCollisionAttack){
        collisionAttack = newCollisionAttack;
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

    void takeCollisionDamageFrom(Character* other){
        if(other->isDead()){
            return;
        }
        health -= other->getCollisionAttack();
    }

    double distanceFrom(Character* other){
        return sqrt(pow(other->getX() - x, 2) +  pow(y - other->getY(), 2));
    }

    void render(RenderProps renderProps) override {
        GameWorldRenderer::render(renderProps, this);
    };

};
