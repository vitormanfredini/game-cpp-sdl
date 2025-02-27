#pragma once

#include "../Projectile.h"
// #include "../Character.h"
#include <memory>

class Character;

class IWeapon {

public:
    IWeapon(int updatesBetweenFires): updatesBetweenFires(updatesBetweenFires) {
        updatesUntilFire = updatesBetweenFires;
    }

    void update(){
        if(updatesUntilFire <= 1){
            updatesUntilFire = updatesBetweenFires; 
        }
        updatesUntilFire -= 1;
    }

    std::vector<std::unique_ptr<Projectile>> fire(Character* originChar, Character* towardsChar){
        std::vector<std::unique_ptr<Projectile>> newProjectiles = {};
        if(shouldFire()){
            newProjectiles.push_back(createProjectile(originChar, towardsChar));
        }
        return newProjectiles;
    }

private:
    int updatesBetweenFires;
    int updatesUntilFire;
    virtual std::unique_ptr<Projectile> createProjectile(Character* originChar, Character* towardsChar) = 0;

    bool shouldFire(){
        return updatesUntilFire == 1;
    }
    

};