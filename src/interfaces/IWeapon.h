#pragma once

#include "../Projectile.h"
// #include "../Character.h"
#include <memory>

class Character;

class IWeapon {

public:

    void setFireFrequency(int frequencyInUpdates){
        updatesBetweenFires = frequencyInUpdates;
    }

    void setAttack(float newAttack){
        attack = newAttack;
    }

    void update(){
        countUp += 1;
        if(countUp >= updatesBetweenFires){
            countUp = 0; 
        }
    }

    std::vector<std::unique_ptr<Projectile>> fire(Character* originChar, Character* towardsChar){
        std::vector<std::unique_ptr<Projectile>> newProjectiles = {};
        if(shouldFire()){
            std::unique_ptr<Projectile> newProjectile = createProjectile(originChar, towardsChar);
            newProjectile->setAttack(attack);
            newProjectiles.push_back(std::move(newProjectile));
        }
        return newProjectiles;
    }

protected:
    float attack = 0.0f;

private:
    int updatesBetweenFires = 30;
    int countUp = 0;

    virtual std::unique_ptr<Projectile> createProjectile(Character* originChar, Character* towardsChar) = 0;

    bool shouldFire(){
        return countUp == (updatesBetweenFires - 1);
    }
    

};