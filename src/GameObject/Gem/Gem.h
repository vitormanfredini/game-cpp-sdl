#pragma once

#include <memory>
#include "GameObject/GameObject.h"

class Gem: public GameObject {

private:
    float value = 1.0f;
    bool consumed = false;

public:

    Gem(float value): value(value) {
        //
    }

    float consume(){
        if(consumed){
            // consumed this gem twice
            return 0.0f;
        }
        consumed = true;
        return value;
    }

    bool isConsumed(){
        return consumed;
    }

    std::unique_ptr<Gem> clone() {
        auto copy = std::make_unique<Gem>(value);

        copy->consumed = consumed;
        copy->setSize(getWidth(),getHeight());

        for(std::unique_ptr<RenderComponent>& renderComponent : renderComponents){
            std::unique_ptr<RenderComponent> clonedRenderComponent = renderComponent->clone();
            copy->addRenderComponent(std::move(clonedRenderComponent));
        }
    
        if (collisionComponent) {
            copy->setCollisionComponent(collisionComponent->clone());
        }

        return copy;
    }

};
