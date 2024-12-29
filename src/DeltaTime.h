#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stddef.h>

struct DeltaTimeInfo {
    double deltaTime;
    float multiplier;
};

class DeltaTime {
public:
    DeltaTime() {
        timeNow = SDL_GetPerformanceCounter();
    }

    void update(){
        timeLast = timeNow;
        timeNow = SDL_GetPerformanceCounter();
    }

    DeltaTimeInfo getDeltaTimeInfo(){
        double deltaTime = (double) ((timeNow - timeLast)*1000 / (double) SDL_GetPerformanceFrequency());
        float deltaTimeMultiplier = static_cast<float>(deltaTime) / static_cast<float>(oneFrameInMs);
        return DeltaTimeInfo {
            deltaTime,
            deltaTimeMultiplier
        };
    }

private:
    Uint64 timeNow = 0;
    Uint64 timeLast = 0;
    double oneFrameInMs = 1000 / 60;
};
