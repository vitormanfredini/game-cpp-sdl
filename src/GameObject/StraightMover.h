#pragma once

#include <iostream>
#include "GameObject.h"
#include "MovementComponent.h"

class StraightMover : public MovementComponent {

public:

    MovementDirection getMovementDirection(GameObject& self, GameObject& target) override {
        float deltaX = target.getX() - self.getX();
        float absDeltaX = abs(deltaX);
        float deltaY = target.getY() - self.getY();
        float absDeltaY = abs(deltaY);

        float maxAbsDelta = absDeltaX > absDeltaY ? absDeltaX : absDeltaY;

        return MovementDirection {
            deltaX / maxAbsDelta,
            deltaY / maxAbsDelta,
        }.normalized();
    }

    std::unique_ptr<MovementComponent> clone() const override {
        return std::make_unique<StraightMover>();
    }
};