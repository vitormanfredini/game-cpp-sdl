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

class GameEngine {

public:
    GameEngine(Renderer* renderer, Camera* camera, DeltaTime* deltatime, Input* input): renderer(renderer), camera(camera), deltatime(deltatime), input(input) {
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

    void addEnemies(int howMany){
        for(int c=0;c<howMany;c++){
            std::unique_ptr<Character> newEnemy = std::make_unique<Character>();
            newEnemy->setTexture(renderer->loadTexture("images/enemy.png"));
            newEnemy->setPosition(CharacterUtils::getRandomPositionOutsideScreen(camera->getPositionX(), camera->getPositionY()));
            newEnemy->setSize(0.066f,0.066f);
            newEnemy->setVelocity(0.005f);
            enemies.push_back(std::move(newEnemy));
        }
    }

private:
    Renderer* renderer = nullptr;
    Camera* camera = nullptr;
    DeltaTime* deltatime = nullptr;
    Input* input = nullptr;
    HealthBar* healthBar = nullptr;
    LevelScript* levelScript = nullptr;

    Character* mainChar;
    std::vector<std::unique_ptr<Character>> enemies = {};

    std::vector<std::unique_ptr<Projectile>> projectiles = {};

    MapGenerator* mapGenerator;

    Menu* menu = nullptr;

    bool paused = false;
    bool quitTrigger = false;

    void doUpdate(){

        input->update();

        mainChar->update();
        mainChar->move(input->getMovementDirection());

        camera->update(mainChar->getX(), mainChar->getY());

        std::vector<LevelScriptKeyFrame> keyFrames = levelScript->getCurrentKeyFramesAndDelete(deltatime->getUpdatesCount());
        for(LevelScriptKeyFrame keyFrame : keyFrames){
            addEnemies(keyFrame.enemies);
        }

        mapGenerator->updateGroundTiles(camera->getPositionX(),camera->getPositionY());
        
        if(mainChar->shouldFireProjectile()){
            int closestEnemyIndex = CharacterUtils::getClosestCharacterIndex(enemies, mainChar);
            if(closestEnemyIndex >= 0){
                projectiles.push_back(
                    mainChar->createProjectile(
                        enemies[closestEnemyIndex].get(),
                        renderer->loadTexture("images/projectile.png")
                    )
                );
            }
        }

        for(std::unique_ptr<Projectile>& projectile : projectiles){
            projectile->update();
        }

        for(size_t e=0; e<enemies.size(); e++){
            enemies[e]->moveTowards(mainChar);
            if(mainChar->isCollidingWith(enemies[e].get())){
                mainChar->takeDamageFrom(enemies[e].get());
            }
            
            for(size_t p=0; p<projectiles.size(); p++){
                if(projectiles[p]->isCollidingWith(enemies[e].get())){
                    enemies[e]->takeDamageFrom(projectiles[p].get());
                }
            }

        }

        projectilesGarbageCollector();
        enemiesGarbageCollector();
        

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
