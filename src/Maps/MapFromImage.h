#pragma once

#include <vector>
#include "GameObject/Alignment.h"
#include "GameObject/SpriteRenderer.h"
#include "MapTileType.h"
#include "TextureManager.h"
#include "MapComponent.h"

class MapFromImage : public MapComponent {

public:

    MapFromImage(TextureManager* textureManager, const char* mapImageFile, int resolution) {
        
        groundTilesColumns = resolution;

        imagePixels = BinaryResourceLoader::toRGBAPixelData(mapImageFile);
        middleX = imagePixels.size() / 2;
        middleY = imagePixels[0].size() / 2;

        renderComponentsPrototypes[MapTileType::Ground] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/stage1/floor_gray.png"),
            Alignment::BottomLeft
        );

        renderComponentsPrototypes[MapTileType::Ground2] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/stage1/floor_gray_dark.png"),
            Alignment::BottomLeft
        );

        renderComponentsPrototypes[MapTileType::Water] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/water.png"),
            Alignment::BottomLeft
        );
    }


private:

    std::vector<std::vector<RGBAPixel>> imagePixels;
    int middleX, middleY;

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
        for(size_t c=0;c<getNumTilesCoverOneBlock();c++){
            int blockTileX = c % groundTilesColumns;
            int blockTileY = std::floor(static_cast<float>(c) / static_cast<float>(groundTilesColumns));

            std::unique_ptr<GameObject> tile = std::make_unique<GameObject>();
            tile->addRenderComponent(renderComponentsPrototypes[decideTileType(blockX, blockY, blockTileX, blockTileY)]->clone());
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

    MapTileType decideTileType(int blockX, int blockY, int blockTileX, int blockTileY){
        int pixelX = middleX + (blockX * groundTilesColumns) + blockTileX;
        int pixelY = middleY + (blockY * groundTilesColumns) + blockTileY;
        pixelX = wrap(pixelX,imagePixels.size());
        pixelY = wrap(pixelY,imagePixels[0].size());
        RGBAPixel pixel = imagePixels[pixelX][pixelY];

        if(pixel.r > 200){
            return MapTileType::Ground2;
        }
        if(pixel.b > 200){
            return MapTileType::Water;
        }
        return MapTileType::Ground;
    }

    int wrap(int value, int max) {
        return ((value % max) + max) % max;
    }

};
