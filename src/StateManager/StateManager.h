#pragma once

#include "MainState.h"
#include "GameplayState.h"

class StateManager {

private:
    MainState mainState = MainState::Intro;
    GameplayState gameplayState = GameplayState::Play;
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

    bool shouldRenderIntro(){
        return mainState == MainState::Intro;
    }

    bool shouldUpdateIntro(){
        return shouldRenderIntro();
    }

    bool shouldUpdateMainMenu(){
        return mainState == MainState::MainMenu;
    }

    bool shouldRenderMainMenu(){
        return shouldUpdateMainMenu();
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
