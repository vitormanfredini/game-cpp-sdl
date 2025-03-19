#pragma once

#include <math.h>
#include <iostream>
#include "MovementComponent.h"

class AngledMover : public MovementComponent {

private:
    double angle;
    double angleInRadians;
    double distanceStartReducingAngle;

public:

    AngledMover(double angle, double distanceStartReducingAngle): angle(angle), angleInRadians(angle * (M_PI / 180)), distanceStartReducingAngle(distanceStartReducingAngle) { };

    MovementDirection getMovementDirection(GameObject& self, GameObject& target) override {
        double distance = self.distanceFrom(&target);

        if(distance < distanceStartReducingAngle){
            return getNormalizedDirectionTowardsTarget(self, target).rotated(angleInRadians);
        }
        
        double ratio = distance / distanceStartReducingAngle;
        return getNormalizedDirectionTowardsTarget(self, target).rotated(angleInRadians * ratio);
    }

    std::unique_ptr<MovementComponent> clone() const override {
        return std::make_unique<AngledMover>(angle, distanceStartReducingAngle);
    }
};
