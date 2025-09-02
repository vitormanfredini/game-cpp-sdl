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

    float lastX = 0.0f;
    float lastY = 0.0f;

    FrameStepper walkingFrameStepper { 4, 20 };
    FrameStepper idleFrameStepper { 2, 50 };

    enum class MovementState {
        Walking,
        Idle
    };

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

        float absDeltaX = std::abs(lastX - character->getX());
        float absDeltaY = std::abs(lastY - character->getY());
        lastX = character->getX();
        lastY = character->getY();

        bool thereIsMovement = absDeltaX > 0.001 || absDeltaY > 0.001;

        if(thereIsMovement && movementState == MovementState::Idle){
            movementState = MovementState::Walking;
            walkingFrameStepper.reset();
        }else if(!thereIsMovement && movementState == MovementState::Walking){
            movementState = MovementState::Idle;
            idleFrameStepper.reset();
        }

        if(movementState == MovementState::Walking){
            walkingFrameStepper.update();
        }else if(movementState == MovementState::Idle){
            idleFrameStepper.update();
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

        auto [offsetX, offsetY] = AlignmentUtils::computeOffsets(alignment, width, height);
        x = x + props.leftOffset - props.cameraPosX + offsetX;
        y = y + props.topOffset - props.cameraPosY + offsetY;


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

        SDL_RenderCopyEx(
            props.sdl_renderer,
            texture,
            &srcRect,
            &dstRect,
            0.0,
            nullptr,
            character->isFacingRight() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL
        );
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<CharacterSpriteAnimationRenderer>(texture, alignment);
    }
};