#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stddef.h>
#include "Timer.h"
#include "GameObject/Character/CharacterType.h"

struct SpawnScheduleKeyFrame {
    int update;
    int enemies;
    CharacterType characterType;
};

class SpawnSchedule {
public:

    void addKeyframe(SpawnScheduleKeyFrame keyFrame){
        schedule.push_back(keyFrame);
    }

    std::vector<SpawnScheduleKeyFrame> popKeyFramesAt(int update){
        std::vector<SpawnScheduleKeyFrame> keyFrames = {};
        for(size_t c=0; c<schedule.size(); c++){
            if(update > schedule[c].update){
                break;
            }
            if(update == schedule[c].update){
                keyFrames.push_back(schedule[c]);
                schedule.erase(schedule.begin() + c);
            }
        }
        return keyFrames;
    }

private:
    std::vector<SpawnScheduleKeyFrame> schedule = {};

};
