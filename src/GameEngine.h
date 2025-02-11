#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>
#include "interfaces/IRenderable.h"
#include "Renderer.h"
#include "Camera.h"
#include "DeltaTime.h"
#include "Input.h"
#include "Character.h"
#include "MapGenerator.h"
#include "CharacterUtils.h"
#include "Menu.h"

class GameEngine {

public:
    GameEngine(Renderer* renderer, Camera* camera, DeltaTime* deltatime, Input* input): renderer(renderer), camera(camera), deltatime(deltatime), input(input) {
        //
    }

    void setMainChar(Character* newMainChar){
        mainChar = newMainChar;
    }

    void setMapGenerator(MapGenerator* newMapGenerator){
        mapGenerator = newMapGenerator;
    }

    void setMenu(Menu* newMenu){
        menu = newMenu;
    }

    void addEnemy(Character* newEnemy){
        enemies.push_back(newEnemy);
    }

    bool isPaused(){
        return paused;
    }

    void triggerQuit(){
        quitTrigger = true;
    }

    bool shouldQuit(){
        return quitTrigger;
    }

    void setPause(bool newPaused){
        paused = newPaused;
    }

    void update(){

        deltatime->update();
        int updatesNeeded = deltatime->getUpdatesNeeded();

        if(paused){
            return;
        }

        for(int update=0;update<updatesNeeded;update++){
            doUpdate();
        }

        // healthBar.setHealth(mainChar.getHealth());

        if(mainChar->getHealth() <= 0.0f){
            triggerQuit();
        }
        
    }

    void render(){

        renderer->clearRenderables();

        std::vector<IRenderable*> groundTiles = mapGenerator->getTiles();
        for(size_t e=0; e<groundTiles.size(); e++){
            renderer->addRenderable(groundTiles[e]);
        }

        for(size_t e=0; e<enemies.size(); e++){
            renderer->addRenderable(enemies[e]);
        }

        renderer->addRenderable(mainChar);

        for(size_t p=0; p<projectiles.size(); p++){
            renderer->addRenderable(projectiles[p]);
        }

        // renderer->addRenderable(&healthBar);

        if(paused){
            renderer->addRenderable(menu);
        }

        renderer->render(
            camera->getPositionX(),
            camera->getPositionY()
        );
    }

private:
    Renderer* renderer;
    Camera* camera;
    DeltaTime* deltatime;
    Input* input;

    Character* mainChar;
    std::vector<Character*> enemies = {};

    std::vector<Projectile*> projectiles = {};

    MapGenerator* mapGenerator;

    Menu* menu;

    bool paused = false;
    bool quitTrigger = false;

    void doUpdate(){

        input->update();

        mainChar->update();
        mainChar->move(input->getMovementDirection());

        camera->update(mainChar->getX(), mainChar->getY());

        mapGenerator->updateGroundTiles(camera->getPositionX(),camera->getPositionY());
        
        if(mainChar->shouldFireProjectile()){
            int closestEnemyIndex = CharacterUtils::getClosestCharacterIndex(enemies, mainChar);
            if(closestEnemyIndex >= 0){
                projectiles.push_back(
                    mainChar->createProjectile(enemies[closestEnemyIndex], renderer->loadTexture("images/projectile.png"))
                );
            }
        }

        for(Projectile* projectile : projectiles){
            projectile->update();
        }

        std::vector<int> diedEnemies = {};

        for(size_t e=0; e<enemies.size(); e++){
            enemies[e]->moveTowards(mainChar);
            if(mainChar->isCollidingWith(enemies[e])){
                mainChar->takeDamageFrom(enemies[e]);
            }
            std::vector<int> diedProjectiles = {};
            for(size_t p=0; p<projectiles.size(); p++){
                if(projectiles[p]->isCollidingWith(enemies[e])){
                    enemies[e]->takeDamageFrom(projectiles[p]);
                }
                if(projectiles[p]->remainingHits() <= 0){
                    diedProjectiles.push_back(p);
                }
            }
            for(int index : diedProjectiles){
                projectiles.erase(projectiles.begin() + index);
            }
            if(enemies[e]->getHealth() <= 0.0f){
                diedEnemies.push_back(e);
            }
        }
        for(int index : diedEnemies){
            enemies.erase(enemies.begin() + index);
            Character* newEnemy = new Character();
            newEnemy->setTexture(renderer->loadTexture("images/enemy.png"));
            newEnemy->setPosition(CharacterUtils::getRandomPositionOutsideScreen(camera->getPositionX(), camera->getPositionY()));
            newEnemy->setSize(0.066f,0.066f);
            newEnemy->setVelocity(0.005f);
            enemies.push_back(newEnemy);
        }
    }

};
