#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <memory>

#include "Renderer.h"
#include "Camera.h"
#include "DeltaTime.h"
#include "Input.h"
#include "GameObject/DebrisFactory.h"
#include "GameObject/Character/Character.h"
#include "GameObject/Character/CharacterUtils.h"
#include "GameObject/Character/CharacterFactory.h"
#include "GameObject/Item/Item.h"
#include "GameObject/Item/ItemFactory.h"
#include "GameObject/Item/ItemId.h"
#include "Maps/MapComponent.h"
#include "GameObject/Ui/Menu.h"
#include "GameObject/Ui/MenuFactory.h"
#include "GameObject/Ui/SplashScreen.h"
#include "Stages/SpawnSchedule.h"
#include "StateManager/StateManager.h"
#include "MouseEventType.h"
#include "Audio/AudioEngine.h"
#include "Maps/BlockObjectManager.h"
#include "GameObject/SplashScreenRenderer.h"

class GameEngine {

public:
    GameEngine(
        Renderer* renderer,
        Camera* camera,
        DeltaTime* deltatime,
        Input* input,
        TextureManager* textureManager,
        StateManager* stateManager,
        MenuFactory* menuFactory,
        UpgradeFactory* upgradeFactory,
        ItemFactory* itemFactory,
        DebrisFactory* debrisFactory,
        AudioEngine* audioEngine,
        WeaponFactory* weaponFactory
    ):
    renderer(renderer),
    camera(camera),
    deltatime(deltatime),
    input(input),
    textureManager(textureManager),
    characterFactory(textureManager,weaponFactory),
    stateManager(stateManager),
    itemFactory(itemFactory),
    audioEngine(audioEngine),
    menuFactory(menuFactory),
    debrisFactory(debrisFactory),
    upgradeFactory(upgradeFactory),
    weaponFactory(weaponFactory)
    {
        menu = std::move(menuFactory->createMainMenu());

        intro.setPosition(0.5f,0.5f);
        intro.setSize(0.3f,0.15f);
        intro.addRenderComponent(std::make_unique<SplashScreenRenderer>(
            textureManager->loadTexture("images/intro.png"),
            textureManager->loadTexture(0,0,0),
            60
        ));

        SDL_Color numbersTextColor = { 148, 148, 129, 255 };
        gameover.setPosition(0.5f,0.5f);
        gameover.setSize(0.3f,0.05f);
        gameover.addRenderComponent(std::make_unique<SplashScreenRenderer>(
            textureManager->drawTextOnTexture(
                textureManager->loadTexture(0,0,0,0,200,30),
                "game over",
                FontStyle::MainMenu,
                numbersTextColor,
                TextRenderMethod::Centered
            ),
            textureManager->loadTexture(0,0,0),
            60
        ));

        pause.setPosition(0.0f,0.0f);
        pause.setSize(1.0f,1.0f);
        pause.addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/pause.png"),
            Alignment::UI
        ));

        stateManager->setOnStartLevelCallback([this](){
            onLevelStart();
        });
    }

    void onLevelStart(){
        audioEngine->startBeat();
        camera->pointTo(mainChar, true);
    }

    void setMainChar(Character* newMainChar){
        mainChar = newMainChar;
    }

    void setHealthBar(GameObject* newHealthBar){
        healthBar = newHealthBar;
    }

    void setGemValueBar(GameObject* newGemValueBar){
        gemValueBar = newGemValueBar;
    }

    void setSpawnSchedule(std::unique_ptr<SpawnSchedule> newSpawnSchedule){
        spawnSchedule = std::move(newSpawnSchedule);
    }

    void setMapComponent(MapComponent* newMapComponent){
        mapComponent = newMapComponent;
    }

    void setStateManager(StateManager* newStateManager){
        stateManager = newStateManager;
    }

    void setMousePointer(GameObject* newMousePointer){
        mousePointer = newMousePointer;
    }

    void update(){

        deltatime->update();
        int updatesNeeded = deltatime->getUpdatesNeeded();

        if(stateManager->shouldUpdateIntro()){
            if(intro.finished()){
                stateManager->setMainState(MainState::MainMenu);
            }
        }

        if(stateManager->shouldUpdateGameOver()){
            if(gameover.finished()){
                stateManager->setMainState(MainState::MainMenu);
            }
        }

        for(int update=0;update<updatesNeeded;update++){
            if(stateManager->shouldUpdateMainMenu()){
                menu->update();
            }
            if(stateManager->shouldUpdateIntro()){
                intro.update();
            }
            if(stateManager->shouldUpdateGameOver()){
                gameover.update();
            }
            if(stateManager->shouldUpdateGameWorld()){
                doGameWorldUpdate();
                if(stateManager->isBossCutscene()){
                    bossCutsceneCurrentUpdate += 1;
                }else{
                    stageUpdatesCountUnpaused += 1;
                }
                
                if(!stateManager->isBossCutscene() && stageUpdatesCountUnpaused == unpausedUpdatesTriggerBossCutscene){
                    triggerCutscene();
                }
                if(bossCutsceneCurrentUpdate >= updatesEndBossCutscene){
                    endBossCutscene();
                }
            }
            if(stateManager->isInsideStage()){
                audioEngine->onUpdateFinished(stageUpdatesCountTotal);
                stageUpdatesCountTotal += 1;
            }
        }

    }

    void triggerCutscene(){
        if(stateManager->isBossCutscene()){
            return;
        }

        stateManager->setLevelState(LevelState::BossCutscene);

        std::unique_ptr<Character> bossEnemy = characterFactory.create(CharacterType::FinalBoss);
        bossEnemy->setPosition(CharacterUtils::getRandomPositionOutsideScreen(camera->getPositionX(), camera->getPositionY()));
        enemies.push_back(std::move(bossEnemy));
        finalBoss = enemies.back().get();

        camera->changeSpeed(Camera::SpeedAlpha::Slow);
    }

    void endBossCutscene(){
        if(!stateManager->isBossCutscene()){
            return;
        }

        stateManager->setLevelState(LevelState::Regular);
        camera->changeSpeed(Camera::SpeedAlpha::Regular);
    }

    void render(){

        renderer->clearRenderables();

        if(stateManager->shouldRenderIntro()){
            renderer->addRenderable(&intro);
        }

        if(stateManager->shouldRenderGameOver()){
            renderer->addRenderable(&gameover);
        }

        if(stateManager->shouldRenderGameWorld()){
            for(GameObject* renderable : mapComponent->getTiles()){
                renderer->addRenderable(renderable);
            }

            for(GameObject* renderable : debris.getNearObjects()){
                renderer->addRenderable(renderable);
            }

            for(std::unique_ptr<Character>& enemy : enemies){
                renderer->addRenderable(enemy.get());
            }

            for(std::unique_ptr<Item>& item : items){
                renderer->addRenderable(item.get());
            }

            renderer->addRenderable(mainChar);

            for(std::unique_ptr<Projectile>& projectile : projectiles){
                renderer->addRenderable(projectile.get());
            }
        }

        if(stateManager->shouldRenderGameplayUi()){
            if(healthBar != nullptr){
                renderer->addRenderable(healthBar);
            }
            if(gemValueBar != nullptr){
                renderer->addRenderable(gemValueBar);
            }
        }

        if(stateManager->isPaused()){
            renderer->addRenderable(&pause);
        }

        if(stateManager->shouldRenderMainMenu()){
            renderer->addRenderable(menu.get());
        }

        if(stateManager->shouldRenderUpgradeMenu()){
            renderer->addRenderable(upgradeMenu.get());
        }

        renderer->addRenderable(mousePointer);

        renderer->render(
            camera->getPositionX(),
            camera->getPositionY()
        );
    }

    void onAdvanceLevel(int level){
        // std::cout << "mainChar is now level " << level << std::endl;
        upgradeMenu = std::move(menuFactory->createUpgradeMenu(mainChar, upgradeFactory));
        stateManager->setGamePlayState(GameplayState::UpgradeMenu);
        input->reset();
        audioEngine->onAdvanceLevel(level);
    }

    void handleKeyboardAndMouseEvent(SDL_Event &event){
        bool isLeftClick = event.button.button == SDL_BUTTON_LEFT;
        bool isRightClick = event.button.button == SDL_BUTTON_RIGHT;

        switch (event.type) {
            case SDL_QUIT:
                stateManager->triggerQuit();
                break;
            case SDL_KEYDOWN:
                if(stateManager->shouldUpdateGameWorld() && !stateManager->isBossCutscene()){
                    input->handleKeyDown(event.key.keysym.sym);
                    if(event.key.keysym.sym==SDLK_ESCAPE){
                        stateManager->pauseToggle();
                        break;
                    }
                }

                if(stateManager->isPaused()){
                    if(event.key.keysym.sym==SDLK_RETURN){
                        stateManager->pauseToggle();
                        break;
                    }
                    if(event.key.keysym.sym==SDLK_ESCAPE){
                        stateManager->triggerQuit();
                        break;
                    }
                }

                break;
            case SDL_KEYUP:
                if(stateManager->shouldUpdateGameWorld()){
                    input->handleKeyUp(event.key.keysym.sym);
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                renderer->getVirtualMouseCoords(&virtualMouseX, &virtualMouseY);
                input->setAimMyMouseVirtualCoords(virtualMouseX, virtualMouseY);
                mousePointer->setPosition(virtualMouseX, virtualMouseY);
                if(stateManager->shouldUpdateMainMenu()){
                    menu->handleMouseEvent(virtualMouseX, virtualMouseY, MouseEventType::Motion);
                    break;
                }
                if(stateManager->shouldUpdateUpgradeMenu()){
                    upgradeMenu->handleMouseEvent(virtualMouseX, virtualMouseY, MouseEventType::Motion);
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                if(!isLeftClick && !isRightClick){
                    break;
                }
                MouseEventType eventType = event.type == SDL_MOUSEBUTTONDOWN ? 
                    (isLeftClick ? MouseEventType::LeftClickDown : MouseEventType::RightClickDown)
                    :
                    (isLeftClick ? MouseEventType::LeftClickUp : MouseEventType::RightClickUp);
                if(stateManager->shouldUpdateMainMenu()){
                    menu->handleMouseEvent(virtualMouseX, virtualMouseY, eventType);
                    break;
                }
                if(stateManager->shouldUpdateUpgradeMenu()){
                    upgradeMenu->handleMouseEvent(virtualMouseX, virtualMouseY, eventType);
                    break;
                }
                break;
            }
        }
    }

    bool shouldQuit(){
        return stateManager->shouldQuit();
    }

private:
    Renderer* renderer = nullptr;
    Camera* camera = nullptr;
    DeltaTime* deltatime = nullptr;
    Input* input = nullptr;
    GameObject* healthBar = nullptr;
    GameObject* gemValueBar = nullptr;
    GameObject* mousePointer = nullptr;
    std::unique_ptr<SpawnSchedule> spawnSchedule;
    TextureManager* textureManager = nullptr;
    CharacterFactory characterFactory;
    StateManager* stateManager = nullptr;
    ItemFactory* itemFactory;
    AudioEngine* audioEngine;
    MenuFactory* menuFactory = nullptr;
    DebrisFactory* debrisFactory = nullptr;
    UpgradeFactory* upgradeFactory = nullptr;
    WeaponFactory* weaponFactory = nullptr;
    
    SplashScreen intro { 1*300 };
    SplashScreen gameover { 1*400 };
    GameObject pause;

    Character* mainChar;
    Character* finalBoss = nullptr;
    std::vector<std::unique_ptr<Character>> enemies;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<Item>> items;
    BlockObjectManager debris;

    MapComponent* mapComponent;

    float virtualMouseX, virtualMouseY;

    int stageUpdatesCountTotal = 0;
    int stageUpdatesCountUnpaused = 0;

    std::unique_ptr<Menu> menu = nullptr;
    std::unique_ptr<Menu> upgradeMenu = nullptr;

    const int unpausedUpdatesTriggerBossCutscene = 200;
    const int updatesEndBossCutscene = 200;
    int bossCutsceneCurrentUpdate = 0;

    void doGameWorldUpdate(){
        if(stateManager->isBossCutscene()){
            bossCustsceneUpdate();
        }else{
            regularGameWorldUpdate();
        }

        mapComponent->update(camera->getPositionX(),camera->getPositionY());
        debris.update(camera->getPositionX(),camera->getPositionY());
    }

    void bossCustsceneUpdate(){
        camera->pointTo(enemies.back().get());
    }

    void regularGameWorldUpdate(){
        input->update();
        MovementDirection aim = input->getAimDirection();

        mainChar->update();
        
        mainChar->moveByInput(input->getInputDirection().normalized());

        for(std::unique_ptr<Item>& item : items){
            item->update();
            item->moveTowards(*mainChar);
            if(mainChar->checkCollision(*item)){
                mainChar->consumeItem(item.get());
            }
        }

        for(std::unique_ptr<Projectile>& projectile : projectiles){
            projectile->update();
        }

        camera->pointTo(mainChar);

        std::vector<SpawnScheduleKeyFrame> keyFrames = spawnSchedule->popKeyFramesAt(stageUpdatesCountUnpaused);
        for(SpawnScheduleKeyFrame keyFrame : keyFrames){
            for(int c=0;c<keyFrame.enemies;c++){
                std::unique_ptr<Character> newEnemy = characterFactory.create(keyFrame.characterType);
                newEnemy->setPosition(CharacterUtils::getRandomPositionOutsideScreen(camera->getPositionX(), camera->getPositionY()));
                enemies.push_back(std::move(newEnemy));
            }
        }

        std::vector<std::unique_ptr<Projectile>> newProjectiles = mainChar->fireWeapons();
        for(std::unique_ptr<Projectile>& projectile : newProjectiles){
            projectile->setDirection(aim.normalized());
            scheduleSoundToNextUpdate(projectile->getSound());
            projectiles.push_back(std::move(projectile));
        }

        if(finalBoss){
            std::vector<std::unique_ptr<Projectile>> bossProjectiles = finalBoss->fireWeapons();
            for(std::unique_ptr<Projectile>& projectile : bossProjectiles){
                scheduleSoundToNextUpdate(projectile->getSound());
                projectile->setFromMainChar(false);
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
                scheduleSoundToNextUpdate(mainChar->getCollisionSound());
            }

            for(size_t p=0; p<projectiles.size(); p++){
                if(projectiles[p]->isFromMainChar() == false){
                    continue;
                }
                if(projectiles[p]->checkCollision(*enemies[e])){
                    enemies[e]->takeDamageFrom(projectiles[p].get());
                }
            }

        }

        for(size_t p=0; p<projectiles.size(); p++){
            if(projectiles[p]->isFromMainChar()){
                continue;
            }
            if(projectiles[p]->checkCollision(*mainChar)){
                mainChar->takeDamageFrom(projectiles[p].get());
            }
        }

        std::vector<int> diedProjectiles = {};
        for(size_t p=0; p<projectiles.size(); p++){
            if(projectiles[p]->isDead()){
                diedProjectiles.push_back(p);
            }
        }
        for(int index : diedProjectiles){
            projectiles.erase(projectiles.begin() + index);
        }

        std::vector<int> diedItems = {};
        for(size_t g=0; g<items.size(); g++){
            if(items[g]->isConsumed()){
                diedItems.push_back(g);
            }
        }
        for(int index : diedItems){
            items.erase(items.begin() + index);
        }

        std::vector<int> diedEnemies = {};
        for(size_t e=0; e<enemies.size(); e++){
            if(enemies[e]->isDead() == false){
                continue;
            }

            if(RandomGenerator::getInstance().getRandom() > 0.1f){
                std::unique_ptr<Item> newGemItem = itemFactory->create(ItemId::Gem);
                newGemItem->setPosition(enemies[e]->getX(),enemies[e]->getY());
                items.push_back(std::move(newGemItem));
            }else{
                std::unique_ptr<Item> newHealthItem = itemFactory->create(ItemId::Health);
                newHealthItem->setPosition(enemies[e]->getX(),enemies[e]->getY());
                items.push_back(std::move(newHealthItem));
            }

            std::vector<DebrisFactory::Type> possibleDebris = enemies[e]->getPossibleDebris();
            if(possibleDebris.size() > 0){
                int debrisIndex = RandomGenerator::getInstance().getRandomInt(0, possibleDebris.size()-1);
                std::unique_ptr<GameObject> newDebris = debrisFactory->create(possibleDebris[debrisIndex]);
                newDebris->setPosition(enemies[e]->getX(), enemies[e]->getY());
                debris.addItem(
                    static_cast<int>(std::round(enemies[e]->getX())),
                    static_cast<int>(std::round(enemies[e]->getY())),
                    std::move(newDebris),
                    true
                );
            }

            diedEnemies.push_back(e);
        }

        if(finalBoss && finalBoss->isDead()){
            finalBoss = nullptr;
        }

        for (auto it = diedEnemies.rbegin(); it != diedEnemies.rend(); ++it) {
            enemies.erase(enemies.begin() + *it);
        }

        if(mainChar->isDead()){
            stateManager->setMainState(MainState::GameOver);
        }

    }

    void scheduleSoundToNextUpdate(int id){
        audioEngine->scheduleSoundToUpdate(id, stageUpdatesCountTotal);
    }

};
