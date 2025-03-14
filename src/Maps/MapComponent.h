#pragma once

#include <vector>
#include "GameObject/GameObject.h"

class MapComponent {

public:
    virtual void update(float cameraPosX, float cameraPosY) = 0;
    virtual std::vector<std::unique_ptr<GameObject>>& getTiles() = 0;

};
