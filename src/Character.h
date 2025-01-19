#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>
#include "BinaryResourceLoader.h"
#include <SDL2/SDL_render.h>
#include "Input.h"
#include "interfaces/ICollidable.h"
#include "interfaces/IRenderable.h"
#include "Projectile.h"

class Character: public ICollidable, public IRenderable {

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

    void move(MovementDirection directions, float deltaTimeMultiplier){
        y += directions.vertical * velocity * deltaTimeMultiplier;
        x += directions.horizontal * velocity * deltaTimeMultiplier;
        updateRect();
    }

    MovementDirection getMovementDirectionTowards(Character* other){
        float deltaX = other->getX() + (other->getWidth() / 2) - (x + (getWidth() / 2));
        float absDeltaX = abs(deltaX);
        float deltaY = other->getY() + (other->getHeight() / 2) - (y + (getHeight() / 2));
        float absDeltaY = abs(deltaY);

        float maxAbsDelta = abs(absDeltaX) > abs(absDeltaY) ? absDeltaX : absDeltaY;

        return MovementDirection {
            deltaX / maxAbsDelta,
            deltaY / maxAbsDelta,
        };
    }

    void moveTowards(Character* other, float deltaTimeMultiplier){
        move(getMovementDirectionTowards(other), deltaTimeMultiplier);
    }

    void takeDamageFrom(Projectile* projectile){
        health -= projectile->getAttack();
        projectile->takeHit();
    }

    void takeDamageFrom(Character* other){
        health -= other->getAttack();
    }

    double distanceFrom(Character* other){
        return sqrt(pow(other->getX() - x, 2) +  pow(y - other->getY(), 2));
    }

    void render(SDL_Renderer* renderer) override {
        if(texture == nullptr){
            return;
        }

        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    bool shouldFireProjectile(){
        return updatesUntilNextFire == 0;
    }

    Projectile* createProjectile(Character* towardsOther, SDL_Texture* texture_projectile){
        Projectile* projectile = new Projectile();
        projectile->setAttack(1.0f);
        projectile->setPosition(
            getX() + (getWidth() / 2.0f),
            getY() + (getHeight() / 2.0f)
        );
        projectile->setTexture(texture_projectile);
        projectile->setDirection(getMovementDirectionTowards(towardsOther));
        projectile->setVelocity(7.0f);
        return projectile;
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
