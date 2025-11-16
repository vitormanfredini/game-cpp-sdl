#pragma once

#include "GameObject/GameObject.h"

class SplashScreen: public GameObject {

private:
    int howManyUpdates;

public:
    SplashScreen(int howManyUpdates): howManyUpdates(howManyUpdates) {
        //
    }

    int getTotalUpdates(){
        return howManyUpdates;
    }

    void update(){
        howManyUpdates -= 1;
        for(std::unique_ptr<RenderComponent>& renderComponent : renderComponents){
            renderComponent->update(*this);
        }
    }

    bool finished(){
        return howManyUpdates <= 0;
    }
};