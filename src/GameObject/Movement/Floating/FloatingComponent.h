#pragma once

#include "GameObject/GameObject.h"
#include "MovementDirection.h"
#include <memory>

class FloatingComponent {

public:
    virtual ~FloatingComponent() = default;
    virtual float getDelta() = 0;
    virtual std::unique_ptr<FloatingComponent> clone() const = 0;
    virtual void update() {
        //
    }
};
