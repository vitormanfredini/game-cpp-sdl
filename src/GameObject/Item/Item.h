#pragma once

#include <memory>
#include "GameObject/GameObject.h"
#include "ItemId.h"
#include <iostream>

class Item: public GameObject {

private:
    bool consumed = false;
    ItemId itemId;
    float value;
    std::unique_ptr<MovementComponent> movementComponent;

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

    void update(){
        movementComponent->update();
    }

    void setMovementComponent(std::unique_ptr<MovementComponent> mover) {
        movementComponent = std::move(mover);
    }

    void moveTowards(GameObject& other){
        if (movementComponent){
            move(movementComponent->getMovementDirection(*this, other));
        }
    }

    void move(MovementDirection directions){
        y += directions.vertical * 0.001f;
        x += directions.horizontal * 0.001f;
    }

    std::unique_ptr<Item> clone() {
        auto copy = std::make_unique<Item>(itemId, value);

        copy->consumed = consumed;
        copy->setSize(getWidth(),getHeight());
        copy->setZ(z);

        for(std::unique_ptr<RenderComponent>& renderComponent : renderComponents){
            std::unique_ptr<RenderComponent> clonedRenderComponent = renderComponent->clone();
            copy->addRenderComponent(std::move(clonedRenderComponent));
        }
    
        if (collisionComponent) {
            copy->setCollisionComponent(collisionComponent->clone());
        }

        if(movementComponent){
            copy->setMovementComponent(movementComponent->clone());
        }

        return copy;
    }

};
