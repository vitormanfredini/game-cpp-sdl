#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "interfaces/ICollidable.h"
#include "interfaces/Transformable.h"

class Projectile: public ICollidable, public IRenderable {

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
