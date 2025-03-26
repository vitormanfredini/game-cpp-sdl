#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>
#include "Renderer.h"
#include "Camera.h"
#include "DeltaTime.h"
#include "Input.h"
#include "GameObject/Character/Character.h"
#include "GameObject/Character/CharacterUtils.h"
#include "GameObject/Character/CharacterFactory.h"
#include "Maps/MapComponent.h"
#include "Menu.h"
#include "Intro.h"
#include <memory>
#include "LevelScript.h"
#include "StateManager/StateManager.h"

class GameEngine {

public:
    GameEngine(Renderer* renderer, Camera* camera, DeltaTime* deltatime, Input* input, TextureManager* textureManager): renderer(renderer), camera(camera), deltatime(deltatime), input(input), textureManager(textureManager), characterFactory(textureManager) {
        intro.setPosition(0.0f,0.0f);
        intro.setSize(1.0f,1.0f);
        intro.addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/intro.png"),
            Alignment::UI
        ));

        pause.setPosition(0.0f,0.0f);
        pause.setSize(1.0f,1.0f);
        pause.addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/pause.png"),
            Alignment::UI
        ));

    }

    void setMainChar(Character* newMainChar){
        mainChar = newMainChar;
    }

    void setHealthBar(GameObject* newHealthBar){
        healthBar = newHealthBar;
    }

    void setLevelScript(LevelScript* newLevelScript){
        levelScript = newLevelScript;
    }

    void setMapComponent(MapComponent* newMapComponent){
        mapComponent = newMapComponent;
    }

    void setMenu(Menu* newMenu){
        menu = newMenu;
    }

    void update(){

        deltatime->update();
        int updatesNeeded = deltatime->getUpdatesNeeded();

        if(stateManager.shouldUpdateIntro()){
            if(intro.finished()){
                stateManager.setMainState(MainState::MainMenu);
            }
        }

        for(int update=0;update<updatesNeeded;update++){
            if(stateManager.shouldUpdateIntro()){
                intro.update();
            }
            if(stateManager.shouldUpdateGameWorld()){
                doGameWorldUpdate();
            }
        }

        if(mainChar->isDead()){
            stateManager.triggerQuit();
        }

    }

    void render(){

        renderer->clearRenderables();

        if(stateManager.shouldRenderIntro()){
            renderer->addRenderable(&intro);
        }

        if(stateManager.shouldRenderGameWorld()){
            for(std::unique_ptr<GameObject>& renderable : mapComponent->getTiles()){
                renderer->addRenderable(renderable.get());
            }
    
            for(std::unique_ptr<Character>& enemy : enemies){
                renderer->addRenderable(enemy.get());
            }
    
            renderer->addRenderable(mainChar);
    
            for(std::unique_ptr<Projectile>& projectile : projectiles){
                renderer->addRenderable(projectile.get());
            }
        }

        if(stateManager.shouldRenderGameplayUi()){
            if(healthBar != nullptr){
                renderer->addRenderable(healthBar);
            }
        }

        if(stateManager.isPaused()){
            renderer->addRenderable(&pause);
        }

        if(stateManager.shouldRenderMainMenu()){
            renderer->addRenderable(menu);
        }

        renderer->render(
            camera->getPositionX(),
            camera->getPositionY()
        );
    }

    void handleKeyboardEvent(SDL_Event &event){
        switch (event.type) {
            case SDL_QUIT:
                stateManager.triggerQuit();
                break;
            case SDL_KEYDOWN:
                if(stateManager.shouldUpdateGameWorld()){
                    input->handleKeyDown(event.key.keysym.sym);
                    if(event.key.keysym.sym==SDLK_ESCAPE){
                        stateManager.pauseToggle();
                        return;
                    }
                }

                if(stateManager.shouldRenderMainMenu()){
                    if(event.key.keysym.sym == SDLK_RETURN){
                        stateManager.setMainState(MainState::Gameplay);
                        return;
                    }
                }
                
                if(stateManager.isPaused()){
                    if(event.key.keysym.sym==SDLK_RETURN){
                        stateManager.pauseToggle();
                        return;
                    }
                    if(event.key.keysym.sym==SDLK_ESCAPE){
                        stateManager.triggerQuit();
                        return;
                    }
                }
                
                break;
            case SDL_KEYUP:
                if(stateManager.shouldUpdateGameWorld()){
                    input->handleKeyUp(event.key.keysym.sym);
                }
                break;
        }
    }

    bool shouldQuit(){
        return stateManager.shouldQuit();
    }

private:
    Renderer* renderer = nullptr;
    Camera* camera = nullptr;
    DeltaTime* deltatime = nullptr;
    Input* input = nullptr;
    GameObject* healthBar = nullptr;
    LevelScript* levelScript = nullptr;
    TextureManager* textureManager = nullptr;
    CharacterFactory characterFactory;
    StateManager stateManager;
    Intro intro { 3*60 };
    GameObject pause;

    Character* mainChar;
    std::vector<std::unique_ptr<Character>> enemies = {};

    std::vector<std::unique_ptr<Projectile>> projectiles = {};

    MapComponent* mapComponent;

    int gameWorldUpdatesCount = 0;

    Menu* menu = nullptr;

    void doGameWorldUpdate(){

        input->update();

        mainChar->update();
        mainChar->move(input->getInputDirections().normalized());

        for(std::unique_ptr<Projectile>& projectile : projectiles){
            projectile->update();
        }

        camera->update(mainChar->getX(), mainChar->getY());

        std::vector<LevelScriptKeyFrame> keyFrames = levelScript->getCurrentKeyFramesAndDelete(gameWorldUpdatesCount);
        for(LevelScriptKeyFrame keyFrame : keyFrames){
            for(int c=0;c<keyFrame.enemies;c++){
                std::unique_ptr<Character> newEnemy = characterFactory.create(keyFrame.characterType);
                newEnemy->setPosition(CharacterUtils::getRandomPositionOutsideScreen(camera->getPositionX(), camera->getPositionY()));
                enemies.push_back(std::move(newEnemy));
            }
        }

        mapComponent->update(camera->getPositionX(),camera->getPositionY());

        int closestEnemyIndex = CharacterUtils::getClosestCharacterIndex(enemies, mainChar);
        if(closestEnemyIndex >= 0){
            std::vector<std::unique_ptr<Projectile>> newProjectiles = mainChar->fire(enemies[closestEnemyIndex].get());

            for(std::unique_ptr<Projectile>& projectile : newProjectiles){
                projectiles.push_back(std::move(projectile));
            }
        }

        for(size_t e=0; e<enemies.size(); e++){
            enemies[e]->update();

            enemies[e]->moveTowards(*mainChar);

            for(size_t oe=0; oe<enemies.size(); oe++){
                if(e == oe){
                    continue;
                }
                if(enemies[e]->checkCollision(*enemies[oe])){
                    enemies[e]->getPushedBy(enemies[oe].get());
                }
            }

            if(mainChar->checkCollision(*enemies[e])){
                mainChar->takeCollisionDamageFrom(enemies[e].get());
                enemies[e]->takeCollisionDamageFrom(mainChar);
            }

            for(size_t p=0; p<projectiles.size(); p++){
                if(projectiles[p]->checkCollision(*enemies[e])){
                    enemies[e]->takeDamageFrom(projectiles[p].get());
                }
            }

        }

        projectilesGarbageCollector();
        enemiesGarbageCollector();

        gameWorldUpdatesCount += 1;
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
