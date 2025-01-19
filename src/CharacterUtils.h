#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "Character.h"
#include <random>

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

    static std::vector<float> getRandomPositionOutsideScreen(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        float x = 0;
        float y = 0;

        if(dis(gen) > 0.5){
            if(dis(gen) > 0.5){
                x = 800*2 + (800*2)*0.1;
            }else{
                x = -(800*2)*0.1;
            }
            y = (600*2) * dis(gen);
        }else{
            x = (800*2) * dis(gen);
            if(dis(gen) > 0.5){
                y = 600*2 + (600*2)*0.1;
            }else{
                y = -(600*2)*0.1;
            }
        }

        return {x,y};
    }

    static Character* createNewCharacter(SDL_Texture* texture, float velocity){
        Character* newEnemy = new Character();
        newEnemy->setTexture(texture);
        std::vector<float> pos = CharacterUtils::getRandomPositionOutsideScreen();
        newEnemy->setPosition(
            pos[0],
            pos[1]
        );
        newEnemy->setVelocity(velocity);
        return newEnemy;
    }

};
