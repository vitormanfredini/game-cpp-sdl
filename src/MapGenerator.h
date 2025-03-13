#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>
#include "GameObject/Alignment.h"
#include "GameObject/SpriteRenderer.h"
#include "MapTileType.h"
#include "TextureManager.h"

class MapGenerator {

public:

    MapGenerator(TextureManager* textureManager) {
        renderComponentsPrototypes[MapTileType::Ground] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/grass.png"),
            Alignment::BottomLeft
        );

        renderComponentsPrototypes[MapTileType::Ground2] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/grass2.png"),
            Alignment::BottomLeft
        );

        renderComponentsPrototypes[MapTileType::Water] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/water.png"),
            Alignment::BottomLeft
        );
    }

    void updateGroundTiles(float cameraPosX, float cameraPosY) {

        int currentBlockX = std::floor(cameraPosX);
        int currentBlockY = std::floor(cameraPosY);
        
        for(int x=currentBlockX-1; x < currentBlockX+2; x++){
            for(int y=currentBlockY-1; y < currentBlockY+2; y++){
                bool blockAlreadyHasGround = false;
                for(int t=0;t<groundTiles.size();t=t+groundTilesCoverOneBlock){
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

private:
    std::vector<std::unique_ptr<GameObject>> groundTiles = {};
    int groundTilesColumns = 8;
    int groundTilesCoverOneBlock = groundTilesColumns * groundTilesColumns;

    std::unordered_map<MapTileType, std::unique_ptr<RenderComponent>> renderComponentsPrototypes;

    void addGroundTiles(int blockX, int blockY){
        float groundLeftX = static_cast<float>(blockX);
        float groundRightX = static_cast<float>(blockX) + 1.0f;
        float groundTopY = static_cast<float>(blockY);
        float groundBottomY = static_cast<float>(blockY) + 1.0f;
        float deltaX = groundRightX - groundLeftX;
        float deltaY = groundBottomY - groundTopY;
        float squareSize = deltaX / static_cast<float>(groundTilesColumns);
        float x = groundLeftX;
        float y = groundTopY;
        for(size_t c=0;c<groundTilesCoverOneBlock;c++){

            std::unique_ptr<GameObject> tile = std::make_unique<GameObject>();
            tile->addRenderComponent(renderComponentsPrototypes[decideTileType(blockX, blockY, x, y)]->clone());
            tile->setPosition(x, y);
            tile->setSize(squareSize, squareSize);
            groundTiles.push_back(std::move(tile));

            x += squareSize;
            if(x >= groundLeftX+deltaX - 0.0001){
                x = groundLeftX;
                y += squareSize;
            }
        }
    }

    MapTileType decideTileType(int blockX, int blockY, float x, float y){
        if(blockX % 3 == 0 && blockY % 2 == 0){
            if(x > 0.3 && x < 0.7 && y > 0.1 && y < 0.4){
                return MapTileType::Water;
            }
        }
        if(blockX % 2 == 0 && blockY % 2 == 0){
            return MapTileType::Ground2;
        }
        return MapTileType::Ground;
    }

};
