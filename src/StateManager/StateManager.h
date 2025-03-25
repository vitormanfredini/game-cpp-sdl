#pragma once

#include "MainState.h"
#include "GameplayState.h"

class StateManager {

private:
    MainState mainState = MainState::Intro;
    GameplayState gameplayState = GameplayState::Play;
    bool paused = false;

public:

    void setMainState(MainState newMainState){
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

};
