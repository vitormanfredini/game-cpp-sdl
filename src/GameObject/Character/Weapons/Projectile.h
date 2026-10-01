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
    int durationInUpdates = 10;
    int soundId = -1;
    float soundGain = 0.0f;
    bool fromMainChar = true;

public:

    bool isFromMainChar(){
        return fromMainChar;
    }

    void setFromMainChar(bool newFromMainChar){
        fromMainChar = newFromMainChar;
    }

    void takeHit(){
        remainingHitsBeforeDisappearing--;
    }

    int remainingHits(){
        return remainingHitsBeforeDisappearing;
    }

    bool isDead(){
        if(durationInUpdates <= 0){
            return true;
        }

        if(remainingHitsBeforeDisappearing <= 0){
            return true;
        }

        return false;
    }

    void setMaxHits(int newMaxHits){
        remainingHitsBeforeDisappearing = newMaxHits;
    }

    void setDirection(MovementDirection newDirection){
        movementDirection = newDirection;
    }

    void setDurationInUpdates(int newDurationInUpdates){
        durationInUpdates = newDurationInUpdates;
    }

    void update() {
        x += movementDirection.horizontal * velocity;
        y += movementDirection.vertical * velocity;
        durationInUpdates -= 1;
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

    void setSound(int newSoundId, float gain){
        float maxGain = 2.0f;
        if(gain < 0.0f){
            soundGain = 0.0f;
        }else if(gain > maxGain){
            soundGain = maxGain;
        }else{
            soundGain = gain;
        }
        soundId = newSoundId;
    }

    int getSound(){
        return soundId;
    }

    float getSoundGain(){
        return soundGain;
    }

};
