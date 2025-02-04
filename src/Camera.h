#pragma once

#include "MovingAverage.h"

class Camera {

private:
    int samplesAverage;
    MovingAverage* averagePosX;
    MovingAverage* averagePosY;
    float averagePosXCache = 0.0f;
    float averagePosYCache = 0.0f;

public:

    Camera(int samples, float startX, float startY): samplesAverage(samples) {
        averagePosX = new MovingAverage(samples);
        averagePosX->fill(startX);
        averagePosY = new MovingAverage(samples);
        averagePosY->fill(startY);
    }

    ~Camera(){
        delete averagePosX;
        delete averagePosY;
    }

    void update(float positionX, float positionY){
        averagePosX->addValue(positionX);
        averagePosY->addValue(positionY);
        averagePosXCache = averagePosX->getAverage();
        averagePosYCache = averagePosY->getAverage();
    }

    float getPositionX(){
        return averagePosXCache;
    }

    float getPositionY(){
        return averagePosYCache;
    }

};
