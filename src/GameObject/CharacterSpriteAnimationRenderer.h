#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"
#include "FrameStepper.h"

class CharacterSpriteAnimationRenderer : public RenderComponent {
private:
    SDL_Texture* texture;
    Alignment alignment;

    int frameWidth;
    int frameHeight;

    FrameStepper walkingFrameStepper { 4, 20 };
    FrameStepper idleFrameStepper { 2, 50 };

    enum class MovementState {
        Walking,
        Idle
    };

    bool facingRight = true;

    MovementState movementState = MovementState::Idle;

public:
    CharacterSpriteAnimationRenderer(SDL_Texture* texture, Alignment alignment) : texture(texture), alignment(alignment) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
        if(textureHeight == 0){
            std::cerr << "CharacterSpriteAnimationRenderer(): couldn't get texture height" << std::endl;
            return;
        }
        if(textureWidth % 4 != 0){
            std::cerr << "CharacterSpriteAnimationRenderer(): texture need to be divisible by 4." << std::endl;
        }
        frameWidth = textureWidth / 4;
        frameHeight = textureHeight / 2;
    }

    void update(GameObject& gameObject) override {

        Character* character = dynamic_cast<Character*>(&gameObject);
        if (!character){
            std::cerr << "CharacterSpriteAnimationRenderer.update(): not Character" << std::endl;
            return;
        };

        MovementDirection direction = character->getCurrentDirections();
        float charAbsVelocity = direction.abs();

        if(std::abs(direction.horizontal) > 0.00001f){
            facingRight = direction.horizontal > 0.0f;
        }

        if(charAbsVelocity > 0.1 && movementState == MovementState::Idle){
            movementState = MovementState::Walking;
            walkingFrameStepper.reset();
        }
        if(charAbsVelocity < 0.01 && movementState == MovementState::Walking){
            movementState = MovementState::Idle;
            idleFrameStepper.reset();
        }

        if(movementState == MovementState::Idle){
            idleFrameStepper.update();
        }
        if(movementState == MovementState::Walking){
            walkingFrameStepper.update();
        }

    }

    void render(GameObject& gameObject, RenderProps props) override {
        if (texture == nullptr) {
            std::cerr << "CharacterSpriteAnimationRenderer: no texture" << std::endl;
            return;
        }

        Character* character = dynamic_cast<Character*>(&gameObject);
        if (!character){
            std::cerr << "CharacterSpriteAnimationRenderer.render(): not Character" << std::endl;
            return;
        };

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

        if(movementState == MovementState::Idle){
            idleFrameStepper.update();
        }
        if(movementState == MovementState::Walking){
            walkingFrameStepper.update();
        }

        int currentFrame = movementState == MovementState::Idle ?
            idleFrameStepper.getFrame()
            :
            walkingFrameStepper.getFrame();

        SDL_Rect srcRect = {
            currentFrame * frameWidth,
            movementState == MovementState::Walking ? 0 : frameHeight,
            frameWidth,
            frameHeight,
        };

        SDL_Rect dstRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };

        // SDL_RenderCopy(props.sdl_renderer, texture, &srcRect, &dstRect);

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
        return std::make_unique<CharacterSpriteAnimationRenderer>(texture, alignment);
    }
};