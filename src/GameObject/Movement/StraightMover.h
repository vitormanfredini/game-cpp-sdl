#pragma once

#include <iostream>
#include "MovementComponent.h"

class StraightMover : public MovementComponent {

public:

    MovementDirection getMovementDirection(GameObject& self, GameObject& target) override {
        return getNormalizedDirectionTowardsTarget(self, target);
    }

    std::unique_ptr<MovementComponent> clone() const override {
        return std::make_unique<StraightMover>();
    }
};