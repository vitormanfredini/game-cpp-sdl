#pragma once

#include "MovingAverage.h"
#include "GameObject/Character/Character.h"

class Camera {
public:

    enum class Speed {
        Regular = 20,
        Slow = 120
    };

    Camera(float startX, float startY):
        averagePosX(static_cast<int>(Speed::Regular)),
        averagePosY(static_cast<int>(Speed::Regular))
    {
        averagePosX.fill(startX);
        averagePosY.fill(startY);
    }

    void changeSpeed(Speed newSpeed){
        if(newSpeed == currentSpeed){
            return;
        }
        currentSpeed = newSpeed;

        float averageX = averagePosX.getAverage();
        averagePosX.resize(static_cast<size_t>(newSpeed));
        averagePosX.fill(averageX);

        float averageY = averagePosY.getAverage();
        averagePosY.resize(static_cast<size_t>(newSpeed));
        averagePosY.fill(averageY);
    }

    void pointTo(Character* character){
        float x = character->getX();
        float y = character->getY() - (character->getHeight() / 2);
        pointTo(x, y);
    }

    void pointTo(float positionX, float positionY){
        averagePosX.addValue(positionX);
        averagePosY.addValue(positionY);
        averagePosXCache = averagePosX.getAverage();
        averagePosYCache = averagePosY.getAverage();
    }

    float getPositionX(){
        return averagePosXCache;
    }

    float getPositionY(){
        return averagePosYCache;
    }

private:
    Speed currentSpeed = Speed::Regular;
    MovingAverage averagePosX;
    MovingAverage averagePosY;
    float averagePosXCache = 0.0f;
    float averagePosYCache = 0.0f;
    Speed speed = Speed::Regular;

};
