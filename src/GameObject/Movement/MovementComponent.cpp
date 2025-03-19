#include "GameObject/GameObject.h"
#include "MovementDirection.h"

MovementDirection MovementComponent::getNormalizedDirectionTowardsTarget(GameObject& self, GameObject& target) {
    float deltaX = target.getX() - self.getX();
    float absDeltaX = abs(deltaX);
    float deltaY = target.getY() - self.getY();
    float absDeltaY = abs(deltaY);

    float maxAbsDelta = absDeltaX > absDeltaY ? absDeltaX : absDeltaY;

    return MovementDirection {
        deltaX / maxAbsDelta,
        deltaY / maxAbsDelta,
    }.normalized();
}
