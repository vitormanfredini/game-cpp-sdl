#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stddef.h>
#include "Timer.h"

class DeltaTime {
public:
    DeltaTime(int _updatesPerSecond) {
        updatesPerSecond = _updatesPerSecond;
        updateTimer = new Timer( 1000.0f / static_cast<float>(updatesPerSecond) );
        timeNow = SDL_GetPerformanceCounter();
    }

    ~DeltaTime(){
        delete updateTimer;
    }

    void update(){
        timeLast = timeNow;
        timeNow = SDL_GetPerformanceCounter();

        double deltaTime = (double) ((timeNow - timeLast)*1000 / (double) SDL_GetPerformanceFrequency());
        updateTimer->update(deltaTime);
    }

    int getUpdatesNeeded(){
        
        int timerLoops = updateTimer->howManyTimesTimerLooped();
        if(timerLoops <= 0){
            return 0;
        }

        // if more than one second of updates are necessary, just update once (at this point the fps is too low for keeping up with time)
        if(timerLoops > updatesPerSecond){
            return 1;
        }

        return timerLoops;
    }

    int getUpdatesPerSecond(){
        return updatesPerSecond;
    }

private:

    int updatesPerSecond;
    Timer* updateTimer;

    Uint64 timeNow = 0;
    Uint64 timeLast = 0;

};
