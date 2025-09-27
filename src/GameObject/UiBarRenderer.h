#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <functional>
#include "../RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"
#include "Character/Character.h"

class UiBarRenderer : public RenderComponent {
private:
    SDL_Texture* barSpritesTexture;
    SDL_Texture* backgroundTexture;
    SDL_Texture* foregroundTexture;
    std::function<float()> getMax;
    std::function<float()> getCurrent;

public:
    UiBarRenderer(
        SDL_Texture* barSpritesTexture,
        SDL_Texture* backgroundTexture,
        SDL_Texture* foregroundTexture,
        std::function<float()> getMax,
        std::function<float()> getCurrent
    ):
        barSpritesTexture(barSpritesTexture),
        backgroundTexture(backgroundTexture),
        foregroundTexture(foregroundTexture),
        getMax(getMax),
        getCurrent(getCurrent)
    {}

    void render(GameObject& gameObject, RenderProps props) override {

        float maxValue = getMax();
        float currentValue = getCurrent();

        float percentage = currentValue / maxValue;

        float x = gameObject.x;
        float y = gameObject.y;
        float width = gameObject.width;
        float height = gameObject.height;

        float borderPixelSize = gameObject.height / 9.0f;

        SDL_Rect borderTopLeftSrcRect = {
            0,
            0,
            2,
            2,
        };
        SDL_Rect borderTopLeftDstRect = {
            static_cast<int>(std::round((x - (2.0f * borderPixelSize)) * props.screenScale)),
            static_cast<int>(std::round((y - (2.0f * borderPixelSize)) * props.screenScale)),
            static_cast<int>(std::round((2.0f * borderPixelSize) * props.screenScale)),
            static_cast<int>(std::round((2.0f * borderPixelSize) * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderTopLeftSrcRect, &borderTopLeftDstRect);

        SDL_Rect borderTopRightSrcRect = {
            3,
            0,
            1,
            2,
        };
        SDL_Rect borderTopRightDstRect = {
            static_cast<int>(std::round(((width * maxValue) + x) * props.screenScale)),
            static_cast<int>(std::round((y - (2.0f * borderPixelSize)) * props.screenScale)),
            static_cast<int>(std::round((1.0f * borderPixelSize) * props.screenScale)),
            static_cast<int>(std::round((2.0f * borderPixelSize) * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderTopRightSrcRect, &borderTopRightDstRect);

        SDL_Rect borderBottomLeftSrcRect = {
            0,
            3,
            2,
            1,
        };
        SDL_Rect borderBottomLeftDstRect = {
            static_cast<int>(std::round((x - (2.0f * borderPixelSize)) * props.screenScale)),
            static_cast<int>(std::round((y + height) * props.screenScale)),
            static_cast<int>(std::round((2.0f * borderPixelSize) * props.screenScale)),
            static_cast<int>(std::round((1.0f * borderPixelSize) * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderBottomLeftSrcRect, &borderBottomLeftDstRect);

        SDL_Rect borderBottomRightSrcRect = {
            3,
            3,
            1,
            1,
        };
        SDL_Rect borderBottomRightDstRect = {
            static_cast<int>(std::round((x + (width * maxValue)) * props.screenScale)),
            static_cast<int>(std::round((y + height) * props.screenScale)),
            static_cast<int>(std::round((1.0f * borderPixelSize) * props.screenScale)),
            static_cast<int>(std::round((1.0f * borderPixelSize) * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderBottomRightSrcRect, &borderBottomRightDstRect);

        SDL_Rect borderLeftSrcRect = {
            0,
            2,
            2,
            1,
        };
        SDL_Rect borderLeftDstRect = {
            static_cast<int>(std::round((x - (2.0f * borderPixelSize)) * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round((2.0f * borderPixelSize) * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderLeftSrcRect, &borderLeftDstRect);

        SDL_Rect borderRightSrcRect = {
            3,
            2,
            1,
            1,
        };
        SDL_Rect borderRightDstRect = {
            static_cast<int>(std::round((x + (width * maxValue)) * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round((1.0f * borderPixelSize) * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderRightSrcRect, &borderRightDstRect);

        SDL_Rect borderTopSrcRect = {
            2,
            0,
            1,
            2,
        };
        SDL_Rect borderTopDstRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round((y - (2.0f * borderPixelSize)) * props.screenScale)),
            static_cast<int>(std::round((width * maxValue) * props.screenScale)),
            static_cast<int>(std::round((2.0f * borderPixelSize) * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderTopSrcRect, &borderTopDstRect);

        SDL_Rect borderBottomSrcRect = {
            2,
            3,
            1,
            1,
        };
        SDL_Rect borderBottomDstRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round((y + height) * props.screenScale)),
            static_cast<int>(std::round((width * maxValue) * props.screenScale)),
            static_cast<int>(std::round((1.0f * borderPixelSize) * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderBottomSrcRect, &borderBottomDstRect);


        SDL_Rect bgRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * maxValue * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, backgroundTexture, NULL, &bgRect);

        SDL_Rect fgRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(percentage * width * maxValue * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, foregroundTexture, NULL, &fgRect);

    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<UiBarRenderer>(barSpritesTexture, backgroundTexture, foregroundTexture, getMax, getCurrent);
    }

};