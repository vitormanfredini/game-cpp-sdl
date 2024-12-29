#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"

class Character {

private:
    SDL_Rect rect = {0, 0, 200, 120};
    SDL_Texture* texture = nullptr;

    float velocity = 5.0f;
    float x = 0.0f;
    float y = 0.0f;

    void updateRect(){
        rect.x = round(x);
        rect.y = round(y);
    }

public:

    void setTexture(SDL_Texture* _texture){
        texture = _texture;
    }

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
        updateRect();
    }

    void setVelocity(float newVelocity) {
        velocity = newVelocity;
    }

    void move(MovementDirections directions, float deltaTimeMultiplier){
        y += directions.vertical * velocity * deltaTimeMultiplier;
        x += directions.horizontal * velocity * deltaTimeMultiplier;
        updateRect();
    }

    void moveTowards(Character& other, float deltaTimeMultiplier){

        float deltaX = other.getX() - x;
        float absDeltaX = abs(deltaX);
        float deltaY = other.getY() - y;
        float absDeltaY = abs(deltaY);

        float maxAbsDelta = abs(absDeltaX) > abs(absDeltaY) ? absDeltaX : absDeltaY;

        move(MovementDirections {
            deltaX / maxAbsDelta,
            deltaY / maxAbsDelta,
        }, deltaTimeMultiplier);
    }

    void draw(SDL_Renderer *renderer) {
        if(texture == nullptr){
            return;
        }

        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    float getX(){
        return x;
    }

    float getY(){
        return y;
    }
};
