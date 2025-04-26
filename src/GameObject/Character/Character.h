#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include <iomanip>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include <memory>
#include "MovementDirection.h"
#include "GameObject/GameObject.h"
#include "Weapons/WeaponComponent.h"
#include "GameObject/Movement/MovementComponent.h"
#include "GameObject/UpgradeComponent.h"
#include "StatComponent.h"

class CharacterHealthBarRenderer;
class UiHealthBarRenderer;

class Character: public GameObject {

private:
    float collisionAttack = 0.001;
    float weight = 1.0f;

    std::unordered_map<StatType, std::unique_ptr<StatComponent>> stats;
    float health = 1.0f;

    int regenerateHealthUpdateCount = 0;

    std::vector<std::unique_ptr<WeaponComponent>> weapons = {};
    std::unique_ptr<MovementComponent> movementComponent;
    std::vector<std::unique_ptr<UpgradeComponent>> upgradeComponents = {};

    void reprocessStats(){

        for(std::unique_ptr<UpgradeComponent>& upgrade : upgradeComponents){
            if(upgrade->getType() != UpgradeComponent::Type::StatUpgrade){
                continue;
            }

            if (stats.find(upgrade->getStatUpgrade()->getType()) == stats.end()) {
                continue;
            }

            stats[upgrade->getStatUpgrade()->getType()]->setValue(
                stats[upgrade->getStatUpgrade()->getType()]->getInitialValue()
            );
        }

        for(std::unique_ptr<UpgradeComponent>& upgrade : upgradeComponents){
            if(upgrade->getType() != UpgradeComponent::Type::StatUpgrade){
                continue;
            }

            if (stats.find(upgrade->getStatUpgrade()->getType()) == stats.end()) {
                continue;
            }

            stats[upgrade->getStatUpgrade()->getType()]->add(upgrade->getStatUpgrade()->getValue());
        }
    }

public:

    Character(){
        stats[StatType::MaxHealth] = std::make_unique<StatComponent> (1.0f);
        stats[StatType::BaseSpeed] = std::make_unique<StatComponent> (1.0f);
        stats[StatType::RegenerateHealthAmount] = std::make_unique<StatComponent> (0.0f);
        stats[StatType::RegenerateHealthFasterInUpdates] = std::make_unique<StatComponent> (0.0f);
    }

    void addWeapon(std::unique_ptr<WeaponComponent> weapon){
        weapons.push_back(std::move(weapon));
    }

    void addUpgradeComponent(std::unique_ptr<UpgradeComponent> upgradeComponent){
        upgradeComponents.push_back(std::move(upgradeComponent));
        reprocessStats();
    }

    void update(){

        regenerateHealthUpdateCount += 1;
        int updatesUntilNextRegen = (240) - std::round(stats[StatType::RegenerateHealthFasterInUpdates]->getValue());
        if(regenerateHealthUpdateCount >= updatesUntilNextRegen){
            regenerateHealthUpdateCount = 0;
            addHealth(stats[StatType::RegenerateHealthAmount]->getValue());
        }

        for(std::unique_ptr<WeaponComponent>& weapon : weapons){
            weapon->update();
        }

        if(movementComponent){
            movementComponent->update();
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

    void setInitialMaxHealth(float newInitialMaxHealth){
        stats[StatType::MaxHealth]->setInitialValue(newInitialMaxHealth);
        stats[StatType::MaxHealth]->setValue(newInitialMaxHealth);
    }

    void setInitialHealth(float newInitialHealth){
        health = 0;
        addHealth(newInitialHealth);
    }

    float getHealthPercentage(){
        return health / stats[StatType::MaxHealth]->getValue();
    }

    void addHealth(float value){
        health += value;
        if(health > stats[StatType::MaxHealth]->getValue()){
            health = stats[StatType::MaxHealth]->getValue();
        }
        if(health >= 0.0f){
            
        }
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

    void setInitialBaseSpeed(float initialBaseSpeed){
        stats[StatType::BaseSpeed]->setInitialValue(initialBaseSpeed);
        stats[StatType::BaseSpeed]->setValue(initialBaseSpeed);
    }

    void moveTowards(GameObject& other){
        if (movementComponent){
            move(movementComponent->getMovementDirection(*this, other));
        }
    }

    void move(MovementDirection directions){
        y += directions.vertical * stats[StatType::BaseSpeed]->getValue() * 0.01;
        x += directions.horizontal * stats[StatType::BaseSpeed]->getValue() * 0.01;
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

    void takeDamageFrom(Projectile* projectile){
        if(projectile->isDead()){
            return;
        }
        addHealth(-projectile->getAttack());
        projectile->takeHit();
    }

    void takeCollisionDamageFrom(Character* other){
        if(other->isDead()){
            return;
        }
        addHealth(-other->getCollisionAttack());
    }

    void setMovementComponent(std::unique_ptr<MovementComponent> mover) {
        movementComponent = std::move(mover);
    }

    std::unique_ptr<Character> clone() {
        auto copy = std::make_unique<Character>();

        copy->x = x;
        copy->y = y;
        copy->width = width;
        copy->height = height;
        copy->health = health;
        copy->collisionAttack = collisionAttack;
        copy->weight = weight;

        for(std::unique_ptr<RenderComponent>& renderComponent : renderComponents){
            std::unique_ptr<RenderComponent> clonedRenderComponent = renderComponent->clone();
            copy->addRenderComponent(std::move(clonedRenderComponent));
        }

        for (const auto &stat : stats) {
            copy->stats[stat.first] = std::move(stat.second->clone());
        }
    
        if (collisionComponent) {
            copy->setCollisionComponent(collisionComponent->clone());
        }

        if(movementComponent){
            copy->setMovementComponent(movementComponent->clone());
        }

        copy->weapons.clear();
        for (const auto& weapon : weapons) {
            if (weapon) {
                copy->weapons.push_back(weapon->clone());
            }
        }

        copy->upgradeComponents.clear();
        for (const auto& upgradeComponent : upgradeComponents) {
            if (upgradeComponent) {
                copy->upgradeComponents.push_back(upgradeComponent->clone());
            }
        }

        return copy;
    }

};
