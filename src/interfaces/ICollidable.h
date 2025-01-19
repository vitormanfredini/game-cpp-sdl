#pragma once

class ICollidable {

public:
    virtual float getX() = 0;
    virtual float getY() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual ~ICollidable() = default;

    bool isCollidingWith(ICollidable* other){

        if(other->getX() > getX() + static_cast<float>(getWidth())){
            return false;
        }

        if(other->getY() > getY() + static_cast<float>(getHeight())){
            return false;
        }

        if(other->getX() + static_cast<float>(other->getWidth()) < getX()){
            return false;
        }

        if(other->getY() + static_cast<float>(other->getHeight()) < getY()){
            return false;
        }

        return true;
    }
};