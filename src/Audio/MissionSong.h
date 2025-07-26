#pragma once

#include <vector>

class MissionSong {
public:
    MissionSong(){
        //
    }

    ~MissionSong(){
        //
    }

    void addLoopSound(int level, int soundId){
        if(level >= loopSounds.size()){
            loopSounds.resize(level + 1);
        }
        loopSounds[level].push_back(soundId);
    }

    std::vector<int> getLevelLoopSounds(){
        if(currentLevel >= loopSounds.size()){
            return {};
        }
        return loopSounds[currentLevel];
    }

    void changeLevel(int newLevel){
        currentLevel = newLevel;
    }

private:
    int currentLevel = 0;
    std::vector<std::vector<int>> loopSounds = {{}};
    
};
