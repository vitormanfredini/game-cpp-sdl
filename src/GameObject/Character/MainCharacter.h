#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include <memory>
#include "MovementDirection.h"
#include "GameObject/GameObject.h"
#include "GameObject/Character/Character.h"
#include "Weapons/WeaponComponent.h"
#include "GameObject/Movement/MovementComponent.h"
#include "GameObject/Gem/Gem.h"

class MainCharacter: public Character {

private:
    float currentGemValue = 0.0f;
    int level = 0;
    float currentLevelMax = 1.0f;

    void recalculateCurrentLevelMax(){
        currentLevelMax = static_cast<float>(level + 1);
    }

public:

    MainCharacter(){
        recalculateCurrentLevelMax();
    }

    void consumeGem(Gem* gem){
        currentGemValue += gem->consume();
        if(currentGemValue > currentLevelMax){
            currentGemValue -= currentLevelMax;
            recalculateCurrentLevelMax();
            level += 1;
        }
    }

    float getGemPercentage(){
        return currentGemValue / currentLevelMax;
    }

};
