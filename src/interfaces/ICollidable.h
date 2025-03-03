#pragma once

#include "GameObject.h"

class ICollidable : public virtual GameObject {

protected:
    float collisionBoxWidth, collisionBoxHeight;

public:

    virtual ~ICollidable() = default;

    bool isCollidingWith(ICollidable* other){

        float thisLeftX = getX() - (getCollisionBoxWidth() / 2);
        float otherRightX = other->getX() + (other->getCollisionBoxWidth() / 2);
        if(thisLeftX > otherRightX){
            return false;
        }

        float thisRightX = getX() + (getCollisionBoxWidth() / 2);
        float otherLeftX = other->getX() - (other->getCollisionBoxWidth() / 2);
        if(thisRightX < otherLeftX){
            return false;
        }

        float thisBottomY = getY() + (getCollisionBoxHeight() / 2);
        float otherTopY = other->getY() - (other->getCollisionBoxHeight() / 2);
        if(thisBottomY < otherTopY){
            return false;
        }

        float thisTopY = getY() - (getCollisionBoxHeight() / 2);
        float otherBottomY = other->getY() + (other->getCollisionBoxHeight() / 2);
        if(thisTopY > otherBottomY){
            return false;
        }

        return true;
    }

    void setCollisionBox(float width, float height){
        collisionBoxWidth = width;
        collisionBoxHeight = height;
    }

    float getCollisionBoxWidth(){
        return collisionBoxWidth;
    }

    float getCollisionBoxHeight(){
        return collisionBoxHeight;
    }
};