#pragma once
#include <vector>

class GameObject {
protected:
    float x, y;
    float width, height;

public:

    GameObject()
        : x(0), y(0), width(0), height(0) {}

    GameObject(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

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