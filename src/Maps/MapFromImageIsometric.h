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
        int truncatedX = static_cast<int>(cameraPosX);
        int truncatedY = static_cast<int>(cameraPosY);
        
        if(truncatedX == tilesCacheX && truncatedY == tilesCacheY){
            return;
        }

        for(int x=truncatedX-1; x <= truncatedX + 1; x++){
            for(int y=truncatedY-1; y <= truncatedY + 1; y++){
                for (TileBlock& elem : tileBlocks) {
                    if(elem.x == truncatedX && elem.y == truncatedY){
                        for(std::unique_ptr<GameObject>& tile : elem.tiles){
                            tilesCache.push_back(tile.get());
                        }
                    }
                }
            }
        }

        tilesCacheX = truncatedX;
        tilesCacheY = truncatedY;
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
                moveTileToBlock(std::move(tile),posX,posY);
            }
        }

    }

    std::vector<GameObject*>& getTiles(){
        return tilesCache;
    }

private:

    struct TileBlock {
        int x;
        int y;
        std::vector<std::unique_ptr<GameObject>> tiles;
    };
    std::vector<TileBlock> tileBlocks;
    
    std::vector<GameObject*> tilesCache;
    int tilesCacheX = -9999;
    int tilesCacheY = 9999;

    void moveTileToBlock(std::unique_ptr<GameObject> tile, float x, float y){
        int truncatedX = static_cast<int>(x);
        int truncatedY = static_cast<int>(y);

        for (TileBlock& elem : tileBlocks) {
            if(elem.x == truncatedX && elem.y == truncatedY){
                elem.tiles.push_back(std::move(tile));
                return;
            }
        }

        tileBlocks.push_back({
            truncatedX,
            truncatedY,
            std::vector<std::unique_ptr<GameObject>> {}
        });
        tileBlocks.back().tiles.push_back(std::move(tile));

    }

    std::vector<std::vector<RGBAPixel>> imagePixels;
    float tileWidth, tileHeight, tileHorizontalOffset, tileVerticalOffset, tileAdditionalHorizontalOffsetPerRow, tileAdditionalVerticalOffsetPerColumn;

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
