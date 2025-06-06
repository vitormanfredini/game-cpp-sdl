#pragma once
#include <vector>
#include <memory>
#include "CollisionComponent.h"
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Movement/MovementComponent.h"

class GameObject {
protected:

    std::vector<std::unique_ptr<RenderComponent>> renderComponents;
    std::unique_ptr<CollisionComponent> collisionComponent;

public:

    float x, y;
    float width, height;

    GameObject()
        : x(0), y(0), width(0), height(0) {}

    GameObject(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}

    virtual ~GameObject() = default;

    void addRenderComponent(std::unique_ptr<RenderComponent> render) {
        renderComponents.push_back(std::move(render));
    }

    void setCollisionComponent(std::unique_ptr<CollisionComponent> collider) {
        collisionComponent = std::move(collider);
    }

    virtual void render(RenderProps renderProps) {
        for(std::unique_ptr<RenderComponent>& renderComponent : renderComponents){
            renderComponent->render(*this, renderProps);
        }
    }

    bool checkCollision(GameObject& other) {
        if (collisionComponent){
            return collisionComponent->checkCollision(*this, other);
        }
        return false;
    }

    CollisionComponent* getCollider() const {
        return collisionComponent.get();
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    double distanceFrom(GameObject* other){
        return sqrt(pow(x - other->getX(), 2) +  pow(y - other->getY(), 2));
    }

    void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }

    void setPosition(std::vector<float> pos) {
        x = pos[0];
        y = pos[1];
    }

    void setSize(float newWidth, float newHeight) {
        width = newWidth;
        height = newHeight;
    }
};
