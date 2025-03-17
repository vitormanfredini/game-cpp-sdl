#pragma once
#include "MovementDirection.h"
#include <memory>

class GameObject;

class MovementComponent {

public:
    virtual MovementDirection getMovementDirection(GameObject& self, GameObject& target) = 0;
    virtual ~MovementComponent() = default;
    virtual std::unique_ptr<MovementComponent> clone() const = 0;

};
