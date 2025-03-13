#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include <memory>
#include "MovementDirection.h"
#include "GameObject/GameObject.h"
#include "Weapons/WeaponComponent.h"

class CharacterHealthBarRenderer;
class UiHealthBarRenderer;

class Character: public GameObject {

private:
    float velocity = 0.01f;
    float collisionAttack = 0.001;
    float weight = 1.0f;

    float initialHealth = 1.0f;
    float health = initialHealth;
    float healthPercentage = 1.0f;

    std::vector<std::unique_ptr<WeaponComponent>> weapons = {};

public:

    void addWeapon(std::unique_ptr<WeaponComponent> weapon){
        weapons.push_back(std::move(weapon));
    }

    void update(){
        for(std::unique_ptr<WeaponComponent>& weapon : weapons){
            weapon->update();
        }
    }

    std::vector<std::unique_ptr<Projectile>> fire(Character* towardsChar){
        std::vector<std::unique_ptr<Projectile>> newProjectiles = {};
        for(std::unique_ptr<WeaponComponent>& weapon : weapons){
            for(std::unique_ptr<Projectile>& projectile : weapon->fire(this, towardsChar)){
                newProjectiles.push_back(std::move(projectile));
            }
        }
        return newProjectiles;
    }

    void getPushedBy(Character* other){
        MovementDirection movementDirection = getMovementDirectionTowards(other).normalized();
        float weightRatio = other->getWeight() / getWeight();
        move(-movementDirection * weightRatio);
    }

    void setInitialHealth(float newInitialHealth){
        initialHealth = newInitialHealth;
        setHealth(newInitialHealth);
    }

    void setHealth(float newHealth){
        health = newHealth;
    }

    float getHealthPercentage(){
        return health / initialHealth;
    }

    float getWeight(){
        return weight;
    }

    void setWeight(float newWeight){
        weight = newWeight;
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
        float deltaX = other->getX() - x;
        float absDeltaX = abs(deltaX);
        float deltaY = other->getY() - y;
        float absDeltaY = abs(deltaY);

        float maxAbsDelta = absDeltaX > absDeltaY ? absDeltaX : absDeltaY;

        return MovementDirection {
            deltaX / maxAbsDelta,
            deltaY / maxAbsDelta,
        };
    }

    void moveTowards(Character* other){
        move(getMovementDirectionTowards(other).normalized());
    }

    void takeDamageFrom(Projectile* projectile){
        if(projectile->isDead()){
            return;
        }
        setHealth(health - projectile->getAttack());
        projectile->takeHit();
    }

    void takeCollisionDamageFrom(Character* other){
        if(other->isDead()){
            return;
        }
        setHealth(health - other->getCollisionAttack());
    }

    double distanceFrom(Character* other){
        return sqrt(pow(x - other->getX(), 2) +  pow(y - other->getY(), 2));
    }

    std::unique_ptr<Character> clone() {
        auto copy = std::make_unique<Character>();

        copy->x = x;
        copy->y = y;
        copy->width = width;
        copy->height = height;
        copy->velocity = velocity;
        copy->health = health;
        copy->initialHealth = initialHealth;
        copy->collisionAttack = collisionAttack;
        copy->weight = weight;

        for(std::unique_ptr<RenderComponent>& renderComponent : renderComponents){
            std::unique_ptr<RenderComponent> clonedRenderComponent = renderComponent->clone();
            copy->addRenderComponent(std::move(clonedRenderComponent));
        }
    
        if (collisionComponent) {
            copy->setCollisionComponent(collisionComponent->clone());
        }

        copy->weapons.clear();
        for (const auto& weapon : weapons) {
            if (weapon) {
                copy->weapons.push_back(weapon->clone());
            }
        }

        return copy;
    }

};
