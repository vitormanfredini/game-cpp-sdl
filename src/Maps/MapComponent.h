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
    void update(float cameraPosX, float cameraPosY) {

        int currentBlockX = std::floor(cameraPosX);
        int currentBlockY = std::floor(cameraPosY);
        
        for(int x=currentBlockX-1; x < currentBlockX+2; x++){
            for(int y=currentBlockY-1; y < currentBlockY+2; y++){
                bool blockAlreadyHasGround = false;
                for(int t=0;t<groundTiles.size();t=t+getNumTilesCoverOneBlock()){
                    if(std::round(groundTiles[t]->getX()) == x && std::round(groundTiles[t]->getY()) == y){
                        blockAlreadyHasGround = true;
                        break;
                    }
                }
                if(blockAlreadyHasGround == false){
                    addGroundTiles(x, y);
                }
            }
        }
    }

    std::vector<std::unique_ptr<GameObject>>& getTiles(){
        return groundTiles;
    }

    virtual void addGroundTiles(int blockX, int blockY) = 0;

};
