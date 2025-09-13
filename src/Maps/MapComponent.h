#pragma once

#include <vector>
#include "GameObject/GameObject.h"

class MapComponent {

protected:
    std::vector<std::unique_ptr<GameObject>> groundTiles;
    int groundTilesColumns;

    int getNumTilesCoverOneBlock(){
        return groundTilesColumns * groundTilesColumns;
    }

public:
    virtual void update(float cameraPosX, float cameraPosY) = 0;

    virtual std::vector<std::unique_ptr<GameObject>>& getTiles() = 0;

};
