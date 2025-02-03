#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>
#include "Character.h"

class CharacterUtils {

public:

    static int getClosestCharacterIndex(std::vector<Character*>& chars, Character& refChar) {
        if(chars.size() == 0){
            return -1;
        }

        int index = 0;
        double minDistance = 0;
        for(size_t c=0;c<chars.size();c++){
            double distance = refChar.distanceFrom(chars[c]);
            if(index==0 || distance<minDistance){
                index = c;
                minDistance = distance;
            }
        }

        return index;
    }

    
    static const std::vector<float> getRandomPositionOutsideScreen(){

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        if(dis(gen) > 0.5){
            return {
                dis(gen) > 0.5 ? 1.1f: -0.1f,
                dis(gen)
            };
        }

        return {
            dis(gen),
            dis(gen) > 0.5 ? 1.1f : -0.1f
        };

    }

};
