#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stddef.h>
#include "Timer.h"
#include "EnemyType.h"

struct LevelScriptKeyFrame {
    int frame;
    int enemies;
    EnemyType enemyType;
};

class LevelScript {
public:

    void addKeyframe(LevelScriptKeyFrame keyFrame){
        script.push_back(keyFrame);
    }

    std::vector<LevelScriptKeyFrame> getCurrentKeyFramesAndDelete(int currentFrame){
        std::vector<LevelScriptKeyFrame> currentFrameKeyFrames = {};
        for(size_t c=0; c<script.size(); c++){
            if(currentFrame > script[c].frame){
                break;
            }
            if(currentFrame == script[c].frame){
                currentFrameKeyFrames.push_back(script[c]);
                script.erase(script.begin() + c);
            }
        }
        return currentFrameKeyFrames;
    }

private:
    std::vector<LevelScriptKeyFrame> script = {};

};
