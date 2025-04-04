#pragma once

#include <math.h>
#include <iostream>
#include "MovementComponent.h"

class ZigZagMover : public MovementComponent {

private:
    double maxAngle;
    double maxAngleInRadians;

    double sinCycle = 0.0;
    double sinCycleStep = 0.02;
    double sinCurrentValue = 0.0;

public:

    ZigZagMover(double maxAngle): maxAngle(maxAngle), maxAngleInRadians(maxAngle * (M_PI / 180)) { };

    MovementDirection getMovementDirection(GameObject& self, GameObject& target) override {
        return getNormalizedDirectionTowardsTarget(self, target).rotated(maxAngleInRadians * sinCurrentValue);
    }

    std::unique_ptr<MovementComponent> clone() const override {
        return std::make_unique<ZigZagMover>(maxAngle);
    }

    void update() override {
        sinCycle += sinCycleStep;
        if(sinCycleStep >= 1.0){
            sinCycleStep -= 1.0;
        }
        sinCurrentValue = sin(2.0 * M_PI * sinCycle);
    }
};
