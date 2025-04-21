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
#include "GameObject/Item/Item.h"

class MainCharacter: public Character {

private:
    float currentGemValue = 0.0f;
    int level = 1;
    float currentLevelMax = 1.0f;

    std::function<void(int)> onAdvanceLevelCallback;

    void recalculateCurrentLevelMax(){
        currentLevelMax = static_cast<float>(level);
    }

    void consumeGemValue(float value){
        currentGemValue += value;
        if(currentGemValue > currentLevelMax){
            level += 1;
            currentGemValue -= currentLevelMax;
            recalculateCurrentLevelMax();
            if(onAdvanceLevelCallback){
                onAdvanceLevelCallback(level);
            }
        }
    }

    void consumeHealthValue(float value){
        addHealth(value);
    }

public:

    MainCharacter(){
        recalculateCurrentLevelMax();
    }

    void consumeItem(Item* item){
        float consumedValue = item->consume();
        ItemId itemId = item->getItemId();

        switch (itemId) {
            case ItemId::Gem: {
                consumeGemValue(consumedValue);
            } break;
            case ItemId::Health: {
                consumeHealthValue(consumedValue);
            } break;
            default:
                std::cerr << "consumeItem() unsupported itemId" << std::endl;
                break;
        }
        
    }

    float getGemPercentage(){
        return currentGemValue / currentLevelMax;
    }

    void setAdvanceLevelCallback(std::function<void(int)> onAdvanceLevel) {
        onAdvanceLevelCallback = onAdvanceLevel;
    }

};
