#pragma once

#include <string.h>
#include <iostream>
#include "MovementDirection.h"

class Projectile: public GameObject {

private:
    float velocity = 0.0f;
    MovementDirection movementDirection;
    float attack = 0.0f;
    int remainingHitsBeforeDisappearing = 1;

public:

    void takeHit(){
        remainingHitsBeforeDisappearing--;
    }

    int remainingHits(){
        return remainingHitsBeforeDisappearing;
    }

    bool isDead(){
        return remainingHitsBeforeDisappearing <= 0;
    }

    void setDirection(MovementDirection newDirection){
        movementDirection = newDirection;
    }

    void update() {
        x += movementDirection.horizontal * velocity;
        y += movementDirection.vertical * velocity;
    }

    void setVelocity(float newVelocity){
        velocity = newVelocity;
    }

    void setAttack(float newAttack){
        attack = newAttack;
    }

    float getAttack(){
        return attack;
    }

};
