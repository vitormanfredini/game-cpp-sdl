#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>
#include "interfaces/IRenderable.h"

class MapGenerator {

public:

    void updateGroundTiles(float cameraPosX, float cameraPosY) {

        int currentBlockX = std::floor(cameraPosX);
        int currentBlockY = std::floor(cameraPosY);
        
        for(int x=currentBlockX-1; x < currentBlockX+2; x++){
            for(int y=currentBlockY-1; y < currentBlockY+2; y++){
                bool blockAlreadyHasGround = false;
                for(int t=0;t<groundTiles.size();t=t+groundTilesCoverScreen){
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

    void setGroundTexture(SDL_Texture* texture){
        groundTexture = texture;
    }

    std::vector<IRenderable*>& getTiles(){
        return groundTiles;
    }

private:
    std::vector<IRenderable*> groundTiles = {};
    int groundTilesColumns = 8;
    int groundTilesCoverScreen = groundTilesColumns * groundTilesColumns;
    SDL_Texture* groundTexture;

    void addGroundTiles(int blockX, int blockY){
        int tilesToAdd = groundTilesCoverScreen;
        float groundLeftX = static_cast<float>(blockX);
        float groundRightX = static_cast<float>(blockX) + 1.0f;
        float groundTopY = static_cast<float>(blockY);
        float groundBottomY = static_cast<float>(blockY) + 1.0f;
        float deltaX = groundRightX - groundLeftX;
        float deltaY = groundBottomY - groundTopY;
        float squareSize = deltaX / static_cast<float>(groundTilesColumns);
        float x = groundLeftX;
        float y = groundTopY;
        for(size_t c=0;c<groundTilesCoverScreen;c++){
            IRenderable* ground = new IRenderable();
            ground->setPosition(x, y);
            ground->setTexture(groundTexture);
            ground->setSize(squareSize, squareSize);
            groundTiles.push_back(ground);
            x += squareSize;
            if(x >= groundLeftX+deltaX - 0.0001){
                x = groundLeftX;
                y += squareSize;
            }
        }
    }

};
