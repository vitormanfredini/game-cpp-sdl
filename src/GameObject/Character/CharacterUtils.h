#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "Character.h"

class CharacterUtils {

public:

    static Character* getClosestCharacter(std::vector<std::unique_ptr<Character>>& chars, Character* refChar) {
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

};
