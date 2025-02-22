#pragma once

#include "MovingAverage.h"

class Camera {

private:
    int samplesAverage;
    MovingAverage averagePosX;
    MovingAverage averagePosY;
    float averagePosXCache = 0.0f;
    float averagePosYCache = 0.0f;

public:

    Camera(int samplesForSmoothing, float startX, float startY): samplesAverage(samplesForSmoothing), averagePosX(samplesForSmoothing), averagePosY(samplesForSmoothing) {
        averagePosX.fill(startX);
        averagePosY.fill(startY);
    }

    void update(float positionX, float positionY){
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

};
