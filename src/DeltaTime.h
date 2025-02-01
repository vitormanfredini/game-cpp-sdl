#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stddef.h>
#include "Timer.h"

class DeltaTime {
public:
    DeltaTime() {
        timeNow = SDL_GetPerformanceCounter();
    }

    void update(){
        timeLast = timeNow;
        timeNow = SDL_GetPerformanceCounter();

        double deltaTime = (double) ((timeNow - timeLast)*1000 / (double) SDL_GetPerformanceFrequency());

        timer.update(deltaTime);
    }

    int getUpdatesNeeded(){
        
        int timerLoops = timer.howManyTimesTimerLooped();
        if(timerLoops <= 0){
            return 0;
        }

        if(timerLoops > 60){
            return 1;
        }

        return timerLoops;
    }

private:

    float fps = 60;
    Timer timer { 1000.0 / fps };

    Uint64 timeNow = 0;
    Uint64 timeLast = 0;

};
