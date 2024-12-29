#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "interfaces/ICollidable.h"

class Projectile: public ICollidable {

private:
    SDL_Rect rect = {0, 0, 32, 32};
    SDL_Texture* texture = nullptr;

    float x = 0.0f;
    float y = 0.0f;
    float velocityX = 0.0f;
    float velocityY = 0.0f;

    float attack = 0.0f;

    void updateRect(){
        rect.x = round(x);
        rect.y = round(y);
    }

public:

    void setPosition(float newX, float newY){
        x = newX;
        y = newY;
    }

    void setTexture(SDL_Texture* _texture){
        texture = _texture;
    }

    void update() {
        x += velocityX;
        y += velocityY;
        updateRect();
    }

    void setVelocity(float newVelocityX, float newVelocityY){
        velocityX = newVelocityX;
        velocityY = newVelocityY;
    }

    void draw(SDL_Renderer *renderer) {
        if(texture == nullptr){
            return;
        }

        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    void setAttack(float newAttack){
        attack = newAttack;
    }

    float getAttack(){
        return attack;
    }

    float getX() override {
        return x;
    }

    float getY() override {
        return y;
    }

    int getWidth() override {
        return rect.w;
    }

    int getHeight(){
        return rect.h;
    }

};
