#pragma once

#include "Projectile.h"
#include "WeaponId.h"
#include <memory>

class Character;

class WeaponComponent {

public:

    void setId(WeaponId newId){
        id = newId;
    }

    WeaponId getId(){
        return id;
    }

    void setFireFrequency(int frequencyInUpdates){
        updatesBetweenFires = frequencyInUpdates;
    }

    void setFireSound(int soundId){
        fireSound = soundId;
    }

    int getFireFrequency(){
        return updatesBetweenFires;
    }

    void setAttack(float newAttack){
        attack = newAttack;
    }

    virtual void update(){
        countUp += 1;
        if(countUp >= updatesBetweenFires){
            countUp = 0; 
        }
    }

    virtual std::vector<std::unique_ptr<Projectile>> fire(Character* originChar){
        std::vector<std::unique_ptr<Projectile>> newProjectiles = {};
        if(shouldFire()){
            std::unique_ptr<Projectile> newProjectile = createProjectile(originChar);
            newProjectile->setAttack(attack);
            newProjectile->setSound(fireSound);
            newProjectiles.push_back(std::move(newProjectile));
        }
        return newProjectiles;
    }

    virtual std::unique_ptr<WeaponComponent> clone() const = 0;

protected:
    float attack = 0.0f;
    int fireSound = -1;

private:
    int updatesBetweenFires = 30;
    int countUp = 0;

    WeaponId id;

    virtual std::unique_ptr<Projectile> createProjectile(Character* originChar) = 0;

    bool shouldFire(){
        return countUp == (updatesBetweenFires - 1);
    }
    

};