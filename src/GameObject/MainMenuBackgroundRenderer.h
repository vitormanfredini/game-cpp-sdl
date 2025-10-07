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
    SDL_Texture* nameTexture;
    Alignment alignment;

    int numberColumns = 67;
    int numberRows = numberColumns;

    std::vector<std::vector<int>> numbers;

public:
    MainMenuBackgroundRenderer(
        SDL_Texture* backgroundTexture,
        SDL_Texture* zeroTexture,
        SDL_Texture* oneTexture,
        SDL_Texture* nameTexture,
        Alignment alignment) : 
        backgroundTexture(backgroundTexture),
        zeroTexture(zeroTexture),
        oneTexture(oneTexture),
        nameTexture(nameTexture),
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
                    static_cast<int>(std::round(static_cast<float>(numY) * (width/numberColumns) * numberSizeRatio * 1.0 * props.screenScale)),
                    static_cast<int>(std::round((width/numberColumns) * props.screenScale)),
                    static_cast<int>(std::round((width/numberColumns) * numberSizeRatio * props.screenScale)),
                };

                SDL_RenderCopy(props.sdl_renderer, numbers[numY][numX] == 0 ? zeroTexture : oneTexture, NULL, &rect);
            }
        }

        SDL_Rect nameBackgroundRect = {
            static_cast<int>(std::round(0.089f * props.screenScale)),
            static_cast<int>(std::round(0.150f * props.screenScale)),
            static_cast<int>(std::round(0.463f * props.screenScale)),
            static_cast<int>(std::round(0.0289f * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, backgroundTexture, NULL, &nameBackgroundRect);
        
        SDL_Rect nameRect = {
            static_cast<int>(std::round(0.105f * props.screenScale)),
            static_cast<int>(std::round(0.146f * props.screenScale)),
            static_cast<int>(std::round(0.432f * props.screenScale)),
            static_cast<int>(std::round(0.0289f * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, nameTexture, NULL, &nameRect);

        SDL_Rect button1BackgroundRect = {
            static_cast<int>(std::round(0.089f * props.screenScale)),
            static_cast<int>(std::round(0.320f * props.screenScale)),
            static_cast<int>(std::round(0.149f * props.screenScale)),
            static_cast<int>(std::round(0.0289f * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, backgroundTexture, NULL, &button1BackgroundRect);

        SDL_Rect button2BackgroundRect = {
            static_cast<int>(std::round(0.089f * props.screenScale)),
            static_cast<int>(std::round(0.350f * props.screenScale)),
            static_cast<int>(std::round(0.134f * props.screenScale)),
            static_cast<int>(std::round(0.0289f * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, backgroundTexture, NULL, &button2BackgroundRect);

        SDL_Rect button3BackgroundRect = {
            static_cast<int>(std::round(0.089f * props.screenScale)),
            static_cast<int>(std::round(0.410f * props.screenScale)),
            static_cast<int>(std::round(0.090f * props.screenScale)),
            static_cast<int>(std::round(0.0289f * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, backgroundTexture, NULL, &button3BackgroundRect);
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<MainMenuBackgroundRenderer>(backgroundTexture, zeroTexture, oneTexture, nameTexture, alignment);
    }
};