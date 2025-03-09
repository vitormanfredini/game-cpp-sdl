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
#include <memory>
#include "LevelScript.h"
#include "EnemyFactory.h"

class GameEngine {

public:
    GameEngine(Renderer* renderer, Camera* camera, DeltaTime* deltatime, Input* input, TextureManager* textureManager): renderer(renderer), camera(camera), deltatime(deltatime), input(input), textureManager(textureManager) {
        //
    }

    void setMainChar(Character* newMainChar){
        mainChar = newMainChar;
    }

    void setHealthBar(HealthBar* newHealthBar){
        healthBar = newHealthBar;
    }

    void setLevelScript(LevelScript* newLevelScript){
        levelScript = newLevelScript;
    }

    void setMapGenerator(MapGenerator* newMapGenerator){
        mapGenerator = newMapGenerator;
    }

    void setMenu(Menu* newMenu){
        menu = newMenu;
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

        if(healthBar != nullptr){
            healthBar->setHealth(mainChar->getHealth());
        }

        if(mainChar->getHealth() <= 0.0f){
            triggerQuit();
        }
        
    }

    void render(){

        renderer->clearRenderables();

        for(std::unique_ptr<IRenderable>& renderable : mapGenerator->getTiles()){
            renderer->addRenderable(renderable.get());
        }

        for(std::unique_ptr<Character>& enemy : enemies){
            renderer->addRenderable(enemy.get());
        }

        renderer->addRenderable(mainChar);

        for(std::unique_ptr<Projectile>& projectile : projectiles){
            renderer->addRenderable(projectile.get());
        }

        if(healthBar != nullptr){
            renderer->addRenderable(healthBar);
        }

        if(paused){
            renderer->addRenderable(menu);
        }

        renderer->render(
            camera->getPositionX(),
            camera->getPositionY()
        );
    }

private:
    Renderer* renderer = nullptr;
    Camera* camera = nullptr;
    DeltaTime* deltatime = nullptr;
    Input* input = nullptr;
    HealthBar* healthBar = nullptr;
    LevelScript* levelScript = nullptr;
    TextureManager* textureManager = nullptr;

    Character* mainChar;
    std::vector<std::unique_ptr<Character>> enemies = {};

    std::vector<std::unique_ptr<Projectile>> projectiles = {};

    MapGenerator* mapGenerator;

    int updatesCount = 0;

    Menu* menu = nullptr;

    bool paused = false;
    bool quitTrigger = false;

    void doUpdate(){

        input->update();

        mainChar->update();
        mainChar->move(input->getMovementDirection().normalized());

        for(std::unique_ptr<Projectile>& projectile : projectiles){
            projectile->update();
        }

        camera->update(mainChar->getX(), mainChar->getY());

        std::vector<LevelScriptKeyFrame> keyFrames = levelScript->getCurrentKeyFramesAndDelete(updatesCount);
        for(LevelScriptKeyFrame keyFrame : keyFrames){
            for(int c=0;c<keyFrame.enemies;c++){
                std::unique_ptr<Character> newEnemy = EnemyFactory::create(
                    textureManager,
                    keyFrame.enemyType
                );
                newEnemy->setPosition(CharacterUtils::getRandomPositionOutsideScreen(camera->getPositionX(), camera->getPositionY()));
                enemies.push_back(std::move(newEnemy));
            }
        }

        mapGenerator->updateGroundTiles(camera->getPositionX(),camera->getPositionY());

        int closestEnemyIndex = CharacterUtils::getClosestCharacterIndex(enemies, mainChar);
        if(closestEnemyIndex >= 0){
            std::vector<std::unique_ptr<Projectile>> newProjectiles = mainChar->fire(enemies[closestEnemyIndex].get());

            for(std::unique_ptr<Projectile>& projectile : newProjectiles){
                projectiles.push_back(std::move(projectile));
            }
        }

        for(size_t e=0; e<enemies.size(); e++){
            enemies[e]->moveTowards(mainChar);

            for(size_t oe=0; oe<enemies.size(); oe++){
                if(e == oe){
                    continue;
                }
                if(enemies[e]->isCollidingWith(enemies[oe].get())){
                    enemies[e]->getPushedBy(enemies[oe].get());
                }
            }

            if(mainChar->isCollidingWith(enemies[e].get())){
                mainChar->takeCollisionDamageFrom(enemies[e].get());
                enemies[e]->takeCollisionDamageFrom(mainChar);
            }

            for(size_t p=0; p<projectiles.size(); p++){
                if(projectiles[p]->isCollidingWith(enemies[e].get())){
                    enemies[e]->takeDamageFrom(projectiles[p].get());
                }
            }

        }

        projectilesGarbageCollector();
        enemiesGarbageCollector();

        updatesCount += 1;
    }

    void projectilesGarbageCollector(){
        std::vector<int> diedProjectiles = {};
        for(size_t p=0; p<projectiles.size(); p++){
            if(projectiles[p]->isDead()){
                diedProjectiles.push_back(p);
            }
        }

        for(int index : diedProjectiles){
            projectiles.erase(projectiles.begin() + index);
        }
    }

    void enemiesGarbageCollector(){
        std::vector<int> diedEnemies = {};
        for(size_t e=0; e<enemies.size(); e++){
            if(enemies[e]->isDead()){
                diedEnemies.push_back(e);
            }
        }
        for(int index : diedEnemies){
            enemies.erase(enemies.begin() + index);
        }
    }

};
