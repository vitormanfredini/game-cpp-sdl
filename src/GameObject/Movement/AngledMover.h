#pragma once

#include <math.h>
#include <iostream>
#include "MovementComponent.h"
#include "Utils.h"

class AngledMover : public MovementComponent {

private:
    double angle;
    double angleInRadians;
    double distance90Degrees;
    double distance90DegreesToCustomAngle;
    double distanceAngle;
    double distanceCustomAngleTo90Degrees;

public:

    AngledMover(double angle, double distance90Degrees, double distance90DegreesToCustomAngle, double distanceAngle, double distanceCustomAngleTo90Degrees)
        : 
        angle(angle),
        angleInRadians(Utils::degreesToRadians(angle)),
        distance90Degrees(distance90Degrees),
        distance90DegreesToCustomAngle(distance90DegreesToCustomAngle),
        distanceAngle(distanceAngle),
        distanceCustomAngleTo90Degrees(distanceCustomAngleTo90Degrees)
    {
        //
    };

    MovementDirection getMovementDirection(GameObject& self, GameObject& target) override {
        double distance = self.distanceFrom(&target);
        
        if(distance < distance90Degrees){
            return getNormalizedDirectionTowardsTarget(self, target);
        }

        if(distance < distance90DegreesToCustomAngle){
            double ratio = (distance - distance90Degrees) / (distance90DegreesToCustomAngle - distance90Degrees);
            return getNormalizedDirectionTowardsTarget(self, target).rotated(angleInRadians * ratio);
        }

        if(distance < distanceAngle){
            return getNormalizedDirectionTowardsTarget(self, target).rotated(angleInRadians);
        }

        if(distance < distanceCustomAngleTo90Degrees){
            double ratio = 1.0f - ((distance - distanceAngle) / (distanceCustomAngleTo90Degrees - distanceAngle));
            return getNormalizedDirectionTowardsTarget(self, target).rotated(angleInRadians * ratio);
        }

        return getNormalizedDirectionTowardsTarget(self, target);
        
    }

    std::unique_ptr<MovementComponent> clone() const override {
        return std::make_unique<AngledMover>(angle, distance90Degrees, distance90DegreesToCustomAngle, distanceAngle, distanceCustomAngleTo90Degrees);
    }
};
