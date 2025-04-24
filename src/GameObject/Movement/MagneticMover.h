#pragma once

#include <math.h>
#include <iostream>
#include "MovementComponent.h"

class MagneticMover : public MovementComponent {

private:
    float distanceThreshold = 0.15;
    int updatesAfterDistanceThreshold = 0;
    int bumpUpdates = 12;
    float velocityIncrementPerUpdate = 0.2;
    bool crossedDistanceThreshold = false;

public:

    MovementDirection getMovementDirection(GameObject& self, GameObject& target) override {
        if(crossedDistanceThreshold == false && self.distanceFrom(&target) < distanceThreshold){
            crossedDistanceThreshold = true;
        }
        if(!crossedDistanceThreshold){
            return MovementDirection(0,0);
        }
        return getNormalizedDirectionTowardsTarget(self, target) * (static_cast<float>(updatesAfterDistanceThreshold - bumpUpdates) * velocityIncrementPerUpdate);
    }

    std::unique_ptr<MovementComponent> clone() const override {
        return std::make_unique<MagneticMover>();
    }

    void update() override {
        if(crossedDistanceThreshold){
            updatesAfterDistanceThreshold += 1;
        }
    }
};
