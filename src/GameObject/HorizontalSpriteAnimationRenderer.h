#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"
#include "FrameStepper.h"

class HorizontalSpriteAnimationRenderer : public RenderComponent {
private:
    SDL_Texture* texture;
    Alignment alignment;
    int updatesPerFrame;

    int frameWidth;
    int frameHeight;
    std::unique_ptr<FrameStepper> frameStepper;

public:
    HorizontalSpriteAnimationRenderer(SDL_Texture* texture, Alignment alignment, int updatesPerFrame, int frameWidth) : texture(texture), alignment(alignment), updatesPerFrame(updatesPerFrame), frameWidth(frameWidth) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
        if(textureHeight == 0){
            std::cerr << "HorizontalSpriteAnimationRenderer(): couldn't get texture height" << std::endl;
            return;
        }
        if(textureWidth % frameWidth != 0){
            std::cerr << "HorizontalSpriteAnimationRenderer(): texture width must be evenly divisible by frame width." << std::endl;
            std::cerr << "textureWidth: " << textureWidth << std::endl;
            std::cerr << "frameWidth: " << frameWidth << std::endl;
        }
        frameHeight = textureHeight;
        frameStepper = std::make_unique<FrameStepper>(textureWidth / frameWidth, updatesPerFrame);
    }

    void update(GameObject& gameObject) override {
        frameStepper->update();
    }

    void render(GameObject& gameObject, RenderProps props) override {
        if (texture == nullptr) {
            std::cerr << "HorizontalSpriteAnimationRenderer: no texture" << std::endl;
            return;
        }

        bool facingRight = true;

        Character* character = dynamic_cast<Character*>(&gameObject);
        if (character){
            facingRight = character->isFacingRight();
        }

        float x = gameObject.x;
        float y = gameObject.y;
        float width = gameObject.width;
        float height = gameObject.height;

        x = x + props.leftOffset;
        y = y + props.topOffset;

        if(alignment != Alignment::UI){
            auto [offsetX, offsetY] = AlignmentUtils::computeOffsets(alignment, width, height);
            x = x - props.cameraPosX + offsetX;
            y = y - props.cameraPosY + offsetY;
        }

        SDL_Rect srcRect = {
            frameStepper->getFrame() * frameWidth,
            0,
            frameWidth,
            frameHeight,
        };

        SDL_Rect dstRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };

        SDL_RenderCopyEx(
            props.sdl_renderer,
            texture,
            &srcRect,
            &dstRect,
            0.0,
            nullptr,
            facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL
        );
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<HorizontalSpriteAnimationRenderer>(texture, alignment, frameStepper->getUpdatesPerFrame(), frameWidth);
    }
};