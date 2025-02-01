#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
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

};
