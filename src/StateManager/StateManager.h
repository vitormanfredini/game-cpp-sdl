#pragma once

#include "MainState.h"
#include "GameplayState.h"
#include "LevelState.h"

class StateManager {

private:
    MainState mainState = MainState::Intro;
    GameplayState gameplayState = GameplayState::Play;
    LevelState levelState = LevelState::Regular;
    bool paused = false;
    bool quitTrigger = false;
    std::function<void()> onStartLevelCallback;

public:

    void setMainState(MainState newMainState){
        if(mainState != MainState::Gameplay && newMainState == MainState::Gameplay){
            if(onStartLevelCallback){
                onStartLevelCallback();
            }
        }
        mainState = newMainState;
    }

    void setLevelState(LevelState newLevelState){
        levelState = newLevelState;
    }

    bool isBossCutscene(){
        return mainState == MainState::Gameplay && levelState == LevelState::BossCutscene;
    }

    void setGamePlayState(GameplayState newGamePlayState){
        gameplayState = newGamePlayState;
    }

    void pauseToggle(){
        paused = !paused;
    }

    bool isPaused(){
        return paused;
    }

    bool isInsideStage(){
        return mainState == MainState::Gameplay;
    }

    bool shouldUpdateGameWorld(){
        if(mainState != MainState::Gameplay){
            return false;
        }
        if(gameplayState != GameplayState::Play){
            return false;
        }
        if(paused){
            return false;
        }
        return true;
    }

    bool shouldRenderGameWorld(){
        if(mainState != MainState::Gameplay){
            return false;
        }
        if(paused){
            return false;
        }
        return true;
    }

    bool shouldUpdateGameplayUi(){
        if(mainState != MainState::Gameplay){
            return false;
        }
        if(paused){
            return false;
        }
        return true;
    }

    bool shouldRenderGameplayUi(){
        return shouldUpdateGameplayUi();
    }

    bool shouldUpdateIntro(){
        return mainState == MainState::Intro;
    }

    bool shouldRenderIntro(){
        return shouldUpdateIntro();
    }

    bool shouldUpdateMainMenu(){
        return mainState == MainState::MainMenu;
    }

    bool shouldRenderMainMenu(){
        return shouldUpdateMainMenu();
    }


    bool shouldUpdateGameOver(){
        return mainState == MainState::GameOver;
    }

    bool shouldRenderGameOver(){
        return shouldUpdateGameOver();
    }

    bool shouldUpdateUpgradeMenu(){
        if(mainState != MainState::Gameplay){
            return false;
        }
        return gameplayState == GameplayState::UpgradeMenu;
    }

    bool shouldRenderUpgradeMenu(){
        return shouldUpdateUpgradeMenu();
    }

    void triggerQuit(){
        quitTrigger = true;
    }

    bool shouldQuit(){
        return quitTrigger;
    }

    void setOnStartLevelCallback(std::function<void()> callback) {
        onStartLevelCallback = callback;
    }

};
