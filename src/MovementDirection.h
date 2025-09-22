#pragma once

#include <cmath>

struct MovementDirection {
    float horizontal;
    float vertical;

    MovementDirection(float h = 0.0f, float v = 0.0f) : horizontal(h), vertical(v) {}

    MovementDirection operator-() const {
        return MovementDirection(-horizontal, -vertical);
    }

    MovementDirection operator-(const MovementDirection& other) const {
      return MovementDirection(horizontal - other.horizontal, vertical - other.vertical);
    }

    MovementDirection operator*(float scalar) const {
        return MovementDirection(horizontal * scalar, vertical * scalar);
    }

    MovementDirection operator/(float scalar) const {
        return (scalar != 0) ? MovementDirection(horizontal / scalar, vertical / scalar) : *this;
    }

    MovementDirection normalized() const {
        float length = std::sqrt(horizontal * horizontal + vertical * vertical);
        return (length > 0) ? *this / length : MovementDirection(0, 0);
    }

    MovementDirection rotated(double angle) const {
        return MovementDirection(
            horizontal * std::cos(angle) - vertical * std::sin(angle),
            vertical * std::cos(angle) + horizontal * std::sin(angle)
        );
    }

};
