#pragma once

class GameObject;

class CollisionComponent {

public:
    virtual bool checkCollision(GameObject& self, GameObject& other) = 0;
    virtual ~CollisionComponent() = default;
    virtual std::unique_ptr<CollisionComponent> clone() const = 0;

    virtual float getOffsetX() const { return 0; }
    virtual float getOffsetY() const { return 0; }
    virtual float getWidth() const { return 0; }
    virtual float getHeight() const { return 0; }
};
