#pragma once

#include "GameObject/GameObject.h"
#include "MovementDirection.h"
#include <memory>

class MovementComponent {

public:
    virtual MovementDirection getMovementDirection(GameObject& self, GameObject& target) = 0;
    virtual ~MovementComponent() = default;
    virtual std::unique_ptr<MovementComponent> clone() const = 0;

    MovementDirection getNormalizedDirectionTowardsTarget(GameObject& self, GameObject& target);
};
