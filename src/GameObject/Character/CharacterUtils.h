#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "Character.h"
#include "RandomGenerator.h"

class CharacterUtils {

public:

    static Character* getClosestCharacterIndex(std::vector<std::unique_ptr<Character>>& chars, Character* refChar) {
        if(chars.size() == 0){
            return nullptr;
        }

        int index = 0;
        double minDistance = 9999999;
        for(size_t c=0;c<chars.size();c++){
            double distance = refChar->distanceFrom(chars[c].get());
            if(c == 0 || distance < minDistance){
                index = c;
                minDistance = distance;
            }
        }

        return chars[index].get();
    }

    
    static const std::vector<float> getRandomPositionOutsideScreen(float cameraX, float cameraY){

        float offsetX = cameraX - 0.5f;
        float offsetY = cameraY - 0.5f;

        if(RandomGenerator::getInstance().getRandom() > 0.5){
            return {
                RandomGenerator::getInstance().getRandom() > 0.5 ? (offsetX + 1.1f) : (offsetX - 0.1f),
                offsetY + RandomGenerator::getInstance().getRandom()
            };
        }

        return {
            offsetX + RandomGenerator::getInstance().getRandom(),
            RandomGenerator::getInstance().getRandom() > 0.5 ? (offsetY + 1.1f) : (offsetY - 0.1f)
        };

    }

};
