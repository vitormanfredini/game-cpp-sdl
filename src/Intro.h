#pragma once

#include "GameObject/GameObject.h"

class Intro: public GameObject {

private:
    int howManyUpdates;

public:
    Intro(int howManyUpdates): howManyUpdates(howManyUpdates) {
        //
    }

    void update(){
        howManyUpdates -= 1;
    }

    bool finished(){
        return howManyUpdates <= 0;
    }
};
