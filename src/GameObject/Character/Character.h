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
#include "Upgrade/UpgradeComponent.h"
#include "StatComponent.h"
#include "LevelManager.h"

class CharacterHealthBarRenderer;
class UiHealthBarRenderer;

class Character: public GameObject {

private:
    float collisionAttack = 0.001;
    float weight = 1.0f;

    std::unordered_map<CharacterStat, std::unique_ptr<StatComponent>> stats;
    float health = 1.0f;

    int regenerateHealthUpdateCount = 0;

    std::vector<std::unique_ptr<WeaponComponent>> weapons = {};
    std::unique_ptr<MovementComponent> movementComponent;

    std::unique_ptr<LevelManager> levelManager;

public:

    Character(){
        stats[CharacterStat::MaxHealth] = std::make_unique<StatComponent> (1.0f);
        stats[CharacterStat::BaseSpeed] = std::make_unique<StatComponent> (1.0f);
        stats[CharacterStat::RegenerateHealthAmount] = std::make_unique<StatComponent> (0.0f);
        stats[CharacterStat::RegenerateHealthFasterInUpdates] = std::make_unique<StatComponent> (0.0f);
    }

    void addWeapon(std::unique_ptr<WeaponComponent> weapon){
        weapons.push_back(std::move(weapon));
    }

    void consumeStatUpgrade(StatUpgrade* upgrade){
        if (stats.find(upgrade->getType()) == stats.end()) {
            std::cerr << "addStatUpgrade(): Character doesn't have the Stat for this Upgrade" << std::endl;
            return;
        }

        stats[upgrade->getType()]->add(upgrade->getValue());
    }

    void consumeWeaponUpgrade(WeaponUpgrade* upgrade){
        if (weapons.size() == 0) {
            std::cerr << "consumeWeaponUpgrade(): Character doesn't have any weapons" << std::endl;
            return;
        }

        for (const std::unique_ptr<WeaponComponent>& weapon : weapons) {
            weapon->setFireFrequency(weapon->getFireFrequency() + std::round(upgrade->getValue()));
        }
    }

    void update(){

        regenerateHealthUpdateCount += 1;
        int updatesUntilNextRegen = (240) - std::round(stats[CharacterStat::RegenerateHealthFasterInUpdates]->getValue());
        if(regenerateHealthUpdateCount >= updatesUntilNextRegen){
            regenerateHealthUpdateCount = 0;
            addHealth(stats[CharacterStat::RegenerateHealthAmount]->getValue());
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
        stats[CharacterStat::MaxHealth]->setInitialValue(newInitialMaxHealth);
        stats[CharacterStat::MaxHealth]->setValue(newInitialMaxHealth);
    }

    void setInitialHealth(float newInitialHealth){
        health = 0;
        addHealth(newInitialHealth);
    }

    float getHealthPercentage(){
        return health / stats[CharacterStat::MaxHealth]->getValue();
    }

    void addHealth(float value){
        health += value;
        if(health > stats[CharacterStat::MaxHealth]->getValue()){
            health = stats[CharacterStat::MaxHealth]->getValue();
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
        stats[CharacterStat::BaseSpeed]->setInitialValue(initialBaseSpeed);
        stats[CharacterStat::BaseSpeed]->setValue(initialBaseSpeed);
    }

    void moveTowards(GameObject& other){
        if (movementComponent){
            move(movementComponent->getMovementDirection(*this, other));
        }
    }

    void move(MovementDirection directions){
        y += directions.vertical * stats[CharacterStat::BaseSpeed]->getValue() * 0.01;
        x += directions.horizontal * stats[CharacterStat::BaseSpeed]->getValue() * 0.01;
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

    void setLevelManager(std::unique_ptr<LevelManager> newLevelManager) {
        levelManager = std::move(newLevelManager);
    }

    float getLevelPercentage(){
        if(levelManager){
            return levelManager->getLevelPercentage();
        }
        std::cerr << "getLevelPercentage(): Character does not have a LevelManager" << std::endl;
        return 0.0f;
    }

    void consumeItem(Item* item){
        float consumedValue = item->consume();
        ItemId itemId = item->getItemId();

        switch (itemId) {
            case ItemId::Gem: {
                levelManager->addGemValue(consumedValue);
            } break;
            case ItemId::Health: {
                addHealth(consumedValue);
            } break;
            default:
                std::cerr << "consumeItem() unsupported itemId" << std::endl;
                break;
        }
        
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

        if(levelManager){
            copy->setLevelManager(levelManager->clone());
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
