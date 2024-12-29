#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "interfaces/ICollidable.h"
#include "Projectile.h"

class Character: public ICollidable {

private:
    SDL_Rect rect = {0, 0, 200, 200};
    SDL_Texture* texture = nullptr;

    float velocity = 5.0f;
    float x = 0.0f;
    float y = 0.0f;
    float health = 1.0f;
    float attack = 0.001;

    int fireEveryXUpdates = 30;
    int updatesUntilNextFire = 0;

    void updateRect(){
        rect.x = round(x);
        rect.y = round(y);
    }

public:

    void update(){
        if(updatesUntilNextFire <= 0){
            updatesUntilNextFire = fireEveryXUpdates;
        }
        updatesUntilNextFire -= 1;
    }

    float getHealth(){
        return health;
    }

    float getAttack(){
        return attack;
    }

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

    void takeDamageFrom(float damage){
        health -= damage;
    }

    double distanceFrom(Character& other){
        return sqrt(pow(other.getX() - x, 2) +  pow(y - other.getY(), 2));
    }

    void draw(SDL_Renderer *renderer) {
        if(texture == nullptr){
            return;
        }

        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    bool shouldFireProjectile(){
        return updatesUntilNextFire == 0;
    }

    virtual float getX(){
        return x;
    }

    virtual float getY(){
        return y;
    }

    virtual int getWidth(){
        return rect.w;
    }

    virtual int getHeight(){
        return rect.h;
    }
};
