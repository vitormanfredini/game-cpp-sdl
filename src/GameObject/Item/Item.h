#pragma once

#include <memory>
#include "GameObject/GameObject.h"
#include "ItemId.h"
#include <iostream>

class Item: public GameObject {

private:
    float value = 1.0f;
    bool consumed = false;
    ItemId itemId;

public:

    Item(ItemId itemId, float value): itemId(itemId), value(value) {
        //
    }

    float consume(){
        if(consumed){
            std::cerr << "consumed Item twice" << std::endl;
            return 0.0f;
        }
        consumed = true;
        return value;
    }

    ItemId getItemId(){
        return itemId;
    }

    bool isConsumed(){
        return consumed;
    }

    std::unique_ptr<Item> clone() {
        auto copy = std::make_unique<Item>(itemId, value);

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
