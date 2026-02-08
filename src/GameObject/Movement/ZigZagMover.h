#pragma once

#include <math.h>
#include <iostream>
#include "MovementComponent.h"
#include "Utils.h"

class ZigZagMover : public MovementComponent {

private:
    double maxAngle;
    double maxAngleInRadians;

    double sinCycleStep;
    double sinCycle = 0.0;
    double sinCurrentValue = 0.0;

public:

    ZigZagMover(double maxAngle, double cycleStep): maxAngle(maxAngle), maxAngleInRadians(Utils::degreesToRadians(maxAngle)), sinCycleStep(cycleStep) { };

    MovementDirection getMovementDirection(GameObject& self, GameObject& target) override {
        return getNormalizedDirectionTowardsTarget(self, target).rotated(maxAngleInRadians * sinCurrentValue);
    }

    std::unique_ptr<MovementComponent> clone() const override {
        return std::make_unique<ZigZagMover>(maxAngle, sinCycleStep);
    }

    void update() override {
        sinCycle += sinCycleStep;
        if(sinCycleStep >= 1.0){
            sinCycleStep -= 1.0;
        }
        sinCurrentValue = sin(2.0 * M_PI * sinCycle);
    }
};
