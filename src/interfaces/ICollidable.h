#pragma once

#include "Transformable.h"

class ICollidable : public virtual Transformable {

public:

    virtual ~ICollidable() = default;

    bool isCollidingWith(ICollidable* other){

        if(other->getX() > getX() + getWidth()){
            return false;
        }

        if(other->getY() > getY() + getHeight()){
            return false;
        }

        if(other->getX() + other->getWidth() < getX()){
            return false;
        }

        if(other->getY() + other->getHeight() < getY()){
            return false;
        }

        return true;
    }
};