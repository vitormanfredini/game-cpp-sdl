#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stddef.h>
#include "Timer.h"

struct DeltaTimeInfo {
    double deltaTime;
    float multiplier;
    int updatesNeeded;
};

class DeltaTime {
public:
    DeltaTime() {
        timeNow = SDL_GetPerformanceCounter();
    }

    void update(){
        timeLast = timeNow;
        timeNow = SDL_GetPerformanceCounter();

        deltaTime = (double) ((timeNow - timeLast)*1000 / (double) SDL_GetPerformanceFrequency());

        timer.update(deltaTime);
    }

    DeltaTimeInfo getDeltaTimeInfo(){
        
        int timerLoops = timer.howManyTimesTimerLooped();;
        float deltaTimeMultiplier = (static_cast<float>(deltaTime) / static_cast<float>(timerLoops)) / static_cast<float>(updateFrequencyInMs);

        return DeltaTimeInfo {
            deltaTime,
            deltaTimeMultiplier,
            timer.howManyTimesTimerLooped()
        };
    }

private:

    // game updates 60 times in 1 second
    Timer timer {1000.0 / 60.0};
    const double updateFrequencyInMs = 1000 / 60;

    double deltaTime = 0;
    Uint64 timeNow = 0;
    Uint64 timeLast = 0;

};
