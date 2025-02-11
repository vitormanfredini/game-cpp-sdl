#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stddef.h>
#include "Timer.h"

class DeltaTime {
public:
    DeltaTime(int updatesPerSecond) {
        fps = updatesPerSecond;
        timer = new Timer( 1000.0f / static_cast<float>(fps) );
        timeNow = SDL_GetPerformanceCounter();
    }

    ~DeltaTime(){
        delete timer;
    }

    void update(){
        timeLast = timeNow;
        timeNow = SDL_GetPerformanceCounter();

        double deltaTime = (double) ((timeNow - timeLast)*1000 / (double) SDL_GetPerformanceFrequency());

        timer->update(deltaTime);
    }

    int getUpdatesNeeded(){
        
        int timerLoops = timer->howManyTimesTimerLooped();
        if(timerLoops <= 0){
            return 0;
        }

        // if more than one second of updates are necessary, just update once (at this point the fps is too low for keeping up with time)
        if(timerLoops > fps){
            return 1;
        }

        return timerLoops;
    }

private:

    int fps;
    Timer* timer;

    Uint64 timeNow = 0;
    Uint64 timeLast = 0;

};
