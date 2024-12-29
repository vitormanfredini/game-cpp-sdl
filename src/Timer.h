#pragma once

#include <SDL2/SDL.h>

class Timer {
public:
    Timer(double frequencyMilliseconds) {
        timerFrequencyInMs = frequencyMilliseconds;
    }

    void update(double deltaTime){
        ellapsedTimeSinceLastUpdate += deltaTime;

        loops = floor(static_cast<float>(ellapsedTimeSinceLastUpdate) / static_cast<float>(timerFrequencyInMs));
        if(loops > 0){
            ellapsedTimeSinceLastUpdate -= timerFrequencyInMs * loops;
        }
    }

    int howManyTimesTimerLooped(){
        return loops;
    }

private:
    Uint64 timeNow = 0;
    Uint64 timeLast = 0;
    int loops = 0;
    double timerFrequencyInMs;
    double ellapsedTimeSinceLastUpdate = 0;

};
