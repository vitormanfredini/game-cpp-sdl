#pragma once

#include "GameObject.h"
#include "RenderProps.h"

class RenderComponent {
    public:
        virtual void render(GameObject& gameObject, RenderProps props) = 0;
        virtual ~RenderComponent() = default;
        virtual std::unique_ptr<RenderComponent> clone() const = 0;
        virtual void update(GameObject& gameObject) {
            //
        }
};