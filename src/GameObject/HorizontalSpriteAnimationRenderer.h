#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"

class HorizontalSpriteAnimationRenderer : public RenderComponent {
private:
    SDL_Texture* texture;
    Alignment alignment;
    int updatesPerFrame;
    int updates = -1;
    int numberOfFrames = 1;
    int currentFrame = 0;
    int frameWidth;
    int frameHeight;

public:
    HorizontalSpriteAnimationRenderer(SDL_Texture* texture, Alignment alignment, int updatesPerFrame) : texture(texture), alignment(alignment), updatesPerFrame(updatesPerFrame) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
        if(textureHeight == 0){
            std::cerr << "SpriteRenderer(): couldn't get texture height" << std::endl;
            return;
        }
        numberOfFrames = textureWidth / textureHeight;
        frameWidth = textureHeight;
        frameHeight = textureHeight;
    }

    void update() override {
        updates += 1;
        if(updates >= updatesPerFrame){
            updates = 0;
            currentFrame += 1;
        }
        if(currentFrame >= numberOfFrames){
            currentFrame = 0;
        }
    }

    void render(GameObject& gameObject, RenderProps props) override {
        if (texture == nullptr) {
            std::cerr << "SpriteRenderer: no texture" << std::endl;
            return;
        }

        float x = gameObject.x;
        float y = gameObject.y;
        float width = gameObject.width;
        float height = gameObject.height;

        switch (alignment) {
            case Alignment::BottomUpCentered:
                x = x - props.cameraPosX + props.leftOffset - (width / 2);
                y = y - props.cameraPosY + props.topOffset - height;
                break;
            case Alignment::Centered:
                x = x - props.cameraPosX + props.leftOffset - (width / 2);
                y = y - props.cameraPosY + props.topOffset - (height / 2);
                break;
            case Alignment::TopLeft:
                x = x - props.cameraPosX + props.leftOffset;
                y = y - props.cameraPosY + props.topOffset;
                break;
            case Alignment::BottomLeft:
                x = x - props.cameraPosX + props.leftOffset;
                y = y - props.cameraPosY + props.topOffset - height;
                break;
            case Alignment::TopDownCentered:
                x = x - props.cameraPosX + props.leftOffset - (width / 2);
                y = y - props.cameraPosY + props.topOffset;
                break;
            case Alignment::UI:
                x = x + props.leftOffset;
                y = y + props.topOffset;
                break;

        }

        SDL_Rect srcRect = {
            currentFrame * frameWidth,
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

        SDL_RenderCopy(props.sdl_renderer, texture, &srcRect, &dstRect);
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<HorizontalSpriteAnimationRenderer>(texture, alignment, updatesPerFrame);
    }
};