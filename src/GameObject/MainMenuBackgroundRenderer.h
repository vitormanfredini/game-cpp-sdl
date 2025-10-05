#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"
#include "RandomGenerator.h"

class MainMenuBackgroundRenderer : public RenderComponent {
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* zeroTexture;
    SDL_Texture* oneTexture;
    Alignment alignment;

    int numberColumns = 67;
    int numberRows = numberColumns + 10;

    std::vector<std::vector<int>> numbers;

public:
    MainMenuBackgroundRenderer(
        SDL_Texture* backgroundTexture,
        SDL_Texture* zeroTexture,
        SDL_Texture* oneTexture,
        Alignment alignment) : 
        backgroundTexture(backgroundTexture),
        zeroTexture(zeroTexture),
        oneTexture(oneTexture),
        alignment(alignment) {
            for(int numY=0;numY<numberRows;numY++){
                numbers.push_back({});
                for(int numX=0;numX<numberColumns;numX++){
                    numbers[numY].push_back(RandomGenerator::getInstance().getRandom() > 0.5 ? 0 : 1);
                }
            }
        }

    void update(GameObject& gameObject) override {
        int numberChangesPerUpdate = 20;
        for(int c=0;c<numberChangesPerUpdate;c++){
            int randomX = std::floor(static_cast<float>(numberColumns) * RandomGenerator::getInstance().getRandom());
            int randomY = std::floor(static_cast<float>(numberRows) * RandomGenerator::getInstance().getRandom());
            numbers[randomY][randomX] = numbers[randomY][randomX] == 1 ? 0 : 1;
        }
    }

    void render(GameObject& gameObject, RenderProps props) override {
        float x = gameObject.x;
        float y = gameObject.y;
        float z = gameObject.z;
        float width = gameObject.width;
        float height = gameObject.height;

        x = x + props.leftOffset;
        y = y + props.topOffset - z;

        if(alignment != Alignment::UI){
            auto [offsetX, offsetY] = AlignmentUtils::computeOffsets(alignment, width, height);
            x = x - props.cameraPosX + offsetX;
            y = y - props.cameraPosY + offsetY;
        }

        SDL_Rect rect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };

        SDL_RenderCopy(props.sdl_renderer, backgroundTexture, NULL, &rect);

        float numberSizeRatio = 1.947;

        for(int numY=0;numY<numberRows;numY++){
            for(int numX=0;numX<numberColumns;numX++){
                SDL_Rect rect = {
                    static_cast<int>(std::round(static_cast<float>(numX) * (width/numberColumns) * props.screenScale)),
                    static_cast<int>(std::round(static_cast<float>(numY) * (width/numberColumns) * numberSizeRatio * 0.9 * props.screenScale)),
                    static_cast<int>(std::round((width/numberColumns) * props.screenScale)),
                    static_cast<int>(std::round((width/numberColumns) * numberSizeRatio * props.screenScale)),
                };

                SDL_RenderCopy(props.sdl_renderer, numbers[numY][numX] == 0 ? zeroTexture : oneTexture, NULL, &rect);
            }
        }
        
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<MainMenuBackgroundRenderer>(backgroundTexture, zeroTexture, oneTexture, alignment);
    }
};