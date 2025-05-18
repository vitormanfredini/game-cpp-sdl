#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stddef.h>
#include "Timer.h"
#include "GameObject/Character/CharacterType.h"
#include <SDL2/SDL_mixer.h>
#include "AudioManager.h"

class LevelSongPlayer {
public:

    LevelSongPlayer(AudioManager* audioManager, int loopLengthInUpdates): audioManager(audioManager), loopLengthInUpdates(loopLengthInUpdates) {
        //
    };

    void addLevelLoops(std::vector<Mix_Chunk*> loops){
        levelLoops.push_back(loops);
    }

    void update(int currentUpdateCount){
        playSounds(currentLevel, currentUpdateCount);
    }

    void onLevelUpdate(int level, int currentUpdateCount){
        currentLevel = level;
    }

    void playSounds(int level, int currentUpdateCount){
        if(!isPlaying){
            return;
        }
        std::vector<Mix_Chunk*> notPlaying = {};
        std::vector<Mix_Chunk*> playing = {};
        std::vector<Mix_Chunk*> levelAudios = getLoopsForLevel(level);
        int offset = currentUpdateCount % loopLengthInUpdates;
        if(offset == 0){
            playAudiosWithOffset(levelAudios, 0);
            loopsPlaying = levelAudios;
        }

        for(Mix_Chunk* audio : levelAudios){
            bool found = false;
            for(Mix_Chunk* soundPlaying : loopsPlaying){
                if(soundPlaying == audio){
                    found = true;
                    break;
                }
            }
            if(found){
                playing.push_back(audio);
            }else{
                notPlaying.push_back(audio);
            }
        }

        playAudiosWithOffset(notPlaying, offset);

        loopsPlaying = playing;
        for(Mix_Chunk* audio : notPlaying){
            loopsPlaying.push_back(audio);
        }
    }

    void play(){
        isPlaying = true;
    }

private:
    std::vector<std::vector<Mix_Chunk*>> levelLoops = {};
    std::vector<Mix_Chunk*> loopsPlaying = {};
    int currentLevel = 0;
    int loopLengthInUpdates;
    bool isPlaying = false;

    AudioManager* audioManager;

    std::vector<Mix_Chunk*> getLoopsForLevel(int level){
        if(levelLoops.size() == 0){
            return {};
        }
        if(level >= levelLoops.size()){
            return levelLoops[levelLoops.size()-1];
        }
        return levelLoops[level];
    }

    void playAudiosWithOffset(std::vector<Mix_Chunk*>& audios, int offset){
        for(Mix_Chunk* audio : audios){
            audioManager->playAudio(audio, offset);
        }
    }

};
