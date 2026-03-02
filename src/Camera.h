#pragma once

#include "ExponentialMovingAverage.h"
#include "GameObject/Character/Character.h"
#include "RandomGenerator.h"

class Camera {
public:

    struct SpeedAlpha {
        static constexpr float Regular = 0.1f;
        static constexpr float Slow    = 0.04f;
    };

    Camera(float startX, float startY):
        averagePosX(SpeedAlpha::Regular, startX),
        averagePosY(SpeedAlpha::Regular, startY)
    {}

    void changeSpeed(float newSpeedAlpha){
        averagePosX.setAlpha(newSpeedAlpha);
        averagePosY.setAlpha(newSpeedAlpha);
    }

    void pointTo(Character* character, bool hardMove = false){
        float x = character->getX();
        float y = character->getY() - (character->getHeight() / 2);
        if(hardMove){
            averagePosX.fill(x);
            averagePosY.fill(y);
        }else{
            pointTo(x, y);
        }
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

    const std::vector<float> getRandomPositionRightOutsideScreen(){

        float offsetX = getPositionX() - 0.5f;
        float offsetY = getPositionY() - 0.5f;

        if(RandomGenerator::getInstance().getRandom() > 0.5){
            return {
                RandomGenerator::getInstance().getRandom() > 0.5 ? (offsetX + 1.1f) : (offsetX - 0.1f),
                offsetY + RandomGenerator::getInstance().getRandom()
            };
        }

        return {
            offsetX + RandomGenerator::getInstance().getRandom(),
            RandomGenerator::getInstance().getRandom() > 0.5 ? (offsetY + 1.1f) : (offsetY - 0.1f)
        };

    }

private:
    ExponentialMovingAverage averagePosX;
    ExponentialMovingAverage averagePosY;
    float averagePosXCache = 0.0f;
    float averagePosYCache = 0.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;

};
