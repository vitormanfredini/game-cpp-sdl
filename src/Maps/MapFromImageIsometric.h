#pragma once

#include <vector>
#include "GameObject/Alignment.h"
#include "GameObject/SpriteRenderer.h"
#include "MapTileType.h"
#include "TextureManager.h"
#include "MapComponent.h"

class MapFromImageIsometric : public MapComponent {

public:

    MapFromImageIsometric(TextureManager* textureManager, const char* mapImageFile, float tileWidth, float tileHeight, float tileHorizontalOffset, float tileVerticalOffset, float tileAdditionalHorizontalOffsetPerRow, float tileAdditionalVerticalOffsetPerColumn): tileWidth(tileWidth), tileHeight(tileHeight), tileHorizontalOffset(tileHorizontalOffset), tileVerticalOffset(tileVerticalOffset), tileAdditionalHorizontalOffsetPerRow(tileAdditionalHorizontalOffsetPerRow), tileAdditionalVerticalOffsetPerColumn(tileAdditionalVerticalOffsetPerColumn) {

        imagePixels = BinaryResourceLoader::toRGBAPixelData(mapImageFile);

        renderComponentsPrototypes[MapTileType::Ground] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/stage1/floor_gray.png"),
            Alignment::BottomLeft
        );

        renderComponentsPrototypes[MapTileType::Ground2] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/stage1/floor_gray_dark.png"),
            Alignment::BottomLeft
        );

        renderComponentsPrototypes[MapTileType::Water] = std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/stage1/floor_gray_dark_variation.png"),
            Alignment::BottomLeft
        );

        addAllTiles();
    }

    void update(float cameraPosX, float cameraPosY){

    }

    void addAllTiles(){

        float startingX = static_cast<float>(imagePixels[0].size() / 2) * tileHorizontalOffset * -1.0;
        float startingY = static_cast<float>(imagePixels.size() / 2) * tileVerticalOffset * -1.0;

        for(int x=0; x<imagePixels[0].size(); x++){
            for(int y=0; y<imagePixels.size(); y++){
                std::unique_ptr<GameObject> tile = std::make_unique<GameObject>();
                tile->addRenderComponent(
                    renderComponentsPrototypes[decideTileType(y, x)]->clone()
                );

                float additionalHorizontalOffset = tileAdditionalHorizontalOffsetPerRow * (static_cast<float>(y) - (static_cast<float>(imagePixels.size()) * 0.5f));
                float additionalVerticalOffset = tileAdditionalVerticalOffsetPerColumn * (static_cast<float>(x) - (static_cast<float>(imagePixels[0].size()) * 0.5f));

                float posX = startingX + (static_cast<float>(x) * tileHorizontalOffset) + additionalHorizontalOffset;
                float posY = startingY + (static_cast<float>(y) * tileVerticalOffset) + additionalVerticalOffset;

                tile->setPosition(
                    posX,
                    posY
                );
                tile->setSize(
                    tileWidth,
                    tileHeight
                );
                groundTiles.push_back(std::move(tile));
            }
        }

    }

    std::vector<std::unique_ptr<GameObject>>& getTiles(){
        return groundTiles;
    }

private:

    std::vector<std::vector<RGBAPixel>> imagePixels;
    float tileWidth, tileHeight, tileHorizontalOffset, tileVerticalOffset, tileAdditionalHorizontalOffsetPerRow, tileAdditionalVerticalOffsetPerColumn;
    
    std::vector<std::unique_ptr<GameObject>> groundTiles;

    std::unordered_map<MapTileType, std::unique_ptr<RenderComponent>> renderComponentsPrototypes;

    MapTileType decideTileType(int pixelY, int pixelX){
        RGBAPixel pixel = imagePixels[pixelY][pixelX];

        if(pixel.r > 200){
            return MapTileType::Ground2;
        }
        if(pixel.b > 200){
            return MapTileType::Water;
        }
        return MapTileType::Ground;
    }

};
