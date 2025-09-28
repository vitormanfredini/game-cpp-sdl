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

    int borderTextureWidth, borderTextureHeight;

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
    {
        SDL_QueryTexture(barSpritesTexture, nullptr, nullptr, &borderTextureWidth, &borderTextureHeight);
    }

    void render(GameObject& gameObject, RenderProps props) override {

        float maxValue = getMax();
        float currentValue = getCurrent();

        float percentage = currentValue / maxValue;

        float x = gameObject.x;
        float y = gameObject.y;
        float width = gameObject.width;
        float height = gameObject.height;

        float borderPixelSize = gameObject.height / 9.0f;

        int borderTopPixels = 2;
        int borderRightPixels = 1;
        int borderBottomPixels = 1;
        int borderLeftPixels = 2;

        float borderTopSize = static_cast<float>(borderTopPixels) * borderPixelSize;
        float borderRightSize = static_cast<float>(borderRightPixels) * borderPixelSize;
        float borderBottomSize = static_cast<float>(borderBottomPixels) * borderPixelSize;
        float borderLeftSize = static_cast<float>(borderLeftPixels) * borderPixelSize;

        SDL_Rect borderTopLeftSrcRect = {
            0,
            0,
            borderLeftPixels,
            borderTopPixels,
        };
        SDL_Rect borderTopLeftDstRect = {
            static_cast<int>(std::round((x - borderLeftSize) * props.screenScale)),
            static_cast<int>(std::round((y - borderTopSize) * props.screenScale)),
            static_cast<int>(std::round(borderLeftSize * props.screenScale)),
            static_cast<int>(std::round(borderTopSize * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderTopLeftSrcRect, &borderTopLeftDstRect);

        SDL_Rect borderTopRightSrcRect = {
            borderTextureWidth - 1,
            0,
            borderRightPixels,
            borderTopPixels,
        };
        SDL_Rect borderTopRightDstRect = {
            static_cast<int>(std::round(((width * maxValue) + x) * props.screenScale)),
            static_cast<int>(std::round((y - borderTopSize) * props.screenScale)),
            static_cast<int>(std::round(borderRightSize * props.screenScale)),
            static_cast<int>(std::round(borderTopSize * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderTopRightSrcRect, &borderTopRightDstRect);

        SDL_Rect borderBottomLeftSrcRect = {
            0,
            borderTextureHeight - 1,
            borderLeftPixels,
            borderBottomPixels,
        };
        SDL_Rect borderBottomLeftDstRect = {
            static_cast<int>(std::round((x - borderLeftSize) * props.screenScale)),
            static_cast<int>(std::round((y + height) * props.screenScale)),
            static_cast<int>(std::round(borderLeftSize * props.screenScale)),
            static_cast<int>(std::round(borderBottomSize * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderBottomLeftSrcRect, &borderBottomLeftDstRect);

        SDL_Rect borderBottomRightSrcRect = {
            borderTextureWidth - 1,
            borderTextureHeight - 1,
            1,
            1,
        };
        SDL_Rect borderBottomRightDstRect = {
            static_cast<int>(std::round((x + (width * maxValue)) * props.screenScale)),
            static_cast<int>(std::round((y + height) * props.screenScale)),
            static_cast<int>(std::round(borderRightSize * props.screenScale)),
            static_cast<int>(std::round(borderBottomSize * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderBottomRightSrcRect, &borderBottomRightDstRect);

        SDL_Rect borderLeftSrcRect = {
            0,
            borderTextureHeight - borderTopPixels - borderBottomPixels,
            borderLeftPixels,
            1,
        };
        SDL_Rect borderLeftDstRect = {
            static_cast<int>(std::round((x - borderLeftSize) * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(borderLeftSize * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderLeftSrcRect, &borderLeftDstRect);

        SDL_Rect borderRightSrcRect = {
            borderTextureWidth - borderRightPixels,
            borderTextureHeight - borderTopPixels - borderBottomPixels,
            borderRightPixels,
            1,
        };
        SDL_Rect borderRightDstRect = {
            static_cast<int>(std::round((x + (width * maxValue)) * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(borderRightSize * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderRightSrcRect, &borderRightDstRect);

        SDL_Rect borderTopSrcRect = {
            borderLeftPixels,
            0,
            1,
            borderTopPixels,
        };
        SDL_Rect borderTopDstRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round((y - borderTopSize) * props.screenScale)),
            static_cast<int>(std::round((width * maxValue) * props.screenScale)),
            static_cast<int>(std::round(borderTopSize * props.screenScale)),
        };
        SDL_RenderCopy(props.sdl_renderer, barSpritesTexture, &borderTopSrcRect, &borderTopDstRect);

        SDL_Rect borderBottomSrcRect = {
            borderLeftPixels,
            borderTextureHeight - borderBottomPixels,
            1,
            borderBottomPixels,
        };
        SDL_Rect borderBottomDstRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round((y + height) * props.screenScale)),
            static_cast<int>(std::round((width * maxValue) * props.screenScale)),
            static_cast<int>(std::round(borderBottomSize * props.screenScale)),
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