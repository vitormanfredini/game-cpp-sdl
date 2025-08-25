#pragma once

#include <iostream>
#include "GameObject.h"
#include "CollisionComponent.h"
#include "Alignment.h"

class BoxCollider : public CollisionComponent {

private:
    float width, height, offsetX, offsetY;

public:
    BoxCollider(float w, float h, float offsetX = 0, float offsetY = 0): width(w), height(h), offsetX(offsetX), offsetY(offsetY) {}

    BoxCollider(float w, float h, Alignment alignment): width(w), height(h) {
        auto [computedOffsetX, computedOffsetY] = AlignmentUtils::computeOffsets(alignment, w, h);
        offsetX = computedOffsetX;
        offsetY = computedOffsetY;
    }

    bool checkCollision(GameObject& self, GameObject& other) override {
        CollisionComponent* selfCollider = self.getCollider();
        CollisionComponent* otherCollider = other.getCollider();
        
        if (!selfCollider || !otherCollider) return false;
    
        float selfLeft   = self.x + selfCollider->getOffsetX();
        float selfTop    = self.y + selfCollider->getOffsetY();
        float selfRight  = selfLeft + selfCollider->getWidth();
        float selfBottom = selfTop + selfCollider->getHeight();
    
        float otherLeft   = other.x + otherCollider->getOffsetX();
        float otherTop    = other.y + otherCollider->getOffsetY();
        float otherRight  = otherLeft + otherCollider->getWidth();
        float otherBottom = otherTop + otherCollider->getHeight();
    
        if (selfLeft < otherRight && selfRight > otherLeft &&
            selfTop < otherBottom && selfBottom > otherTop) {
            return true;
        }

        return false;
    }

    float getOffsetX() const override { return offsetX; }
    float getOffsetY() const override { return offsetY; }
    float getWidth() const override { return width; }
    float getHeight() const override { return height; }

    std::unique_ptr<CollisionComponent> clone() const override {
        return std::make_unique<BoxCollider>(width, height, offsetX, offsetY);
    }
};