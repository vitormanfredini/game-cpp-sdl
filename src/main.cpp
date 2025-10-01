#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "SDLUtils.h"
#include "BinaryResourceLoader.h"
#include "Input.h"
#include "DeltaTime.h"
#include "GameObject/Ui/Menu.h"
#include "Weapons/Projectile.h"
#include "GameObject/Character/Character.h"
#include "Renderer.h"
#include "Camera.h"
#include "Maps/MapComponent.h"
#include "GameEngine.h"
#include "Weapons/FireBallThrower.h"
#include "Weapons/Sword.h"
#include "GameObject/SpriteRenderer.h"
#include "GameObject/ButtonRenderer.h"
#include "TextureManager.h"
#include "GameObject/BoxCollider.h"
#include "Maps/MapFromImageIsometric.h"
#include "GameObject/Ui/MenuFactory.h"
#include "Font/FontManager.h"
#include "Audio/AudioEngine.h"
#include "Audio/StageSong.h"
#include "GameObject/CharacterSpriteAnimationRenderer.h"
#include "Stages/SpawnScheduleFactory.h"
#include "GameObject/UiBarRenderer.h"
#include "GameObject/DebrisFactory.h"

int main() {

    Renderer renderer {
        1200 * 2,
        800 * 2
    };
    if(!renderer.init()){
        return -1;
    }

    Input input;
    DeltaTime deltaTime { 60 };

    Camera camera {
        20,
        0.0f,
        0.0f
    };

    FontManager fontManager;
    TextureManager textureManager {renderer.getSDLRenderer(), &fontManager};
    StateManager stateManager;
    ItemFactory itemFactory(&textureManager);
    UpgradeFactory upgradeFactory(&itemFactory);
    MenuFactory menuFactory(&textureManager, &stateManager);
    DebrisFactory debrisFactory(&textureManager);

    AudioEngine audioEngine;
    if (!audioEngine.init()) {
        std::cerr << "Couldn't initialize Audio Engine" << std::endl;
        return -1;
    }

    GameEngine engine {
        &renderer,
        &camera,
        &deltaTime,
        &input,
        &textureManager,
        &stateManager,
        &menuFactory,
        &upgradeFactory,
        &itemFactory,
        &debrisFactory,
        &audioEngine
    };

    StageSong mission1Song;
    mission1Song.addLoopSound(0, audioEngine.loadSound("audio/song1/drums1.wav"));

    mission1Song.addLoopSound(1, audioEngine.loadSound("audio/song1/loop1.wav"));
    mission1Song.addLoopSound(1, audioEngine.loadSound("audio/song1/drums1.wav"));

    mission1Song.addLoopSound(2, audioEngine.loadSound("audio/song1/loop2.wav"));
    mission1Song.addLoopSound(2, audioEngine.loadSound("audio/song1/drums1.wav"));
    
    mission1Song.addLoopSound(3, audioEngine.loadSound("audio/song1/loop3.wav"));
    mission1Song.addLoopSound(3, audioEngine.loadSound("audio/song1/drums1.wav"));

    mission1Song.addLoopSound(4, audioEngine.loadSound("audio/song1/loop4.wav"));
    mission1Song.addLoopSound(4, audioEngine.loadSound("audio/song1/drums1.wav"));
    audioEngine.setStageSong(&mission1Song);

    Character mainChar;
    mainChar.setPosition(0.0f,0.0f);
    float widthRatio = 0.45;
    float height = 0.2;
    mainChar.setSize(height*widthRatio,height);
    mainChar.setZ(height*0.01);
    mainChar.setInitialBaseSpeed(0.5f);
    mainChar.setInitialMaxHealth(1.0f);
    mainChar.setCollisionAttack(0.03f);
    mainChar.setCollisionSound(audioEngine.loadSound("audio/collision.wav"));
    mainChar.addRenderComponent(std::make_unique<ShadowRenderer>(
        textureManager.loadTexture("images/chars/shadow.png"),
        0.017
    ));
    mainChar.addRenderComponent(std::make_unique<CharacterSpriteAnimationRenderer>(
        textureManager.loadTexture("images/chars/vitor_sprites.png"),
        Alignment::BottomUpCentered
    ));
    mainChar.setCollisionComponent(std::make_unique<BoxCollider>(
        0.10f,
        0.033f,
        Alignment::Centered
    ));

    std::unique_ptr<LevelManager> mainCharLevelManager = std::make_unique<LevelManager>();
    mainCharLevelManager->setOnAdvanceLevelCallback([&engine](int level) {
        engine.onAdvanceLevel(level);
    });
    mainChar.setLevelManager(std::move(mainCharLevelManager));
    

    std::unique_ptr<FireBallThrower> weaponFireDogThrower = std::make_unique<FireBallThrower>();
    weaponFireDogThrower->setProjectileTexture(textureManager.loadTexture("images/projectile.png"));
    weaponFireDogThrower->setAttack(0.5f);
    weaponFireDogThrower->setFireFrequency(20);
    weaponFireDogThrower->setFireSound(audioEngine.loadSound("audio/pew.wav"));
    mainChar.addWeapon(std::move(weaponFireDogThrower));

    std::unique_ptr<Sword> weaponSword = std::make_unique<Sword>();
    weaponSword->setProjectileTexture(textureManager.loadTexture("images/projectile.png"));
    weaponSword->setAttack(2.0f);
    weaponSword->setFireFrequency(87);
    weaponSword->setFireSound(audioEngine.loadSound("audio/fu.wav"));
    mainChar.addWeapon(std::move(weaponSword));

    engine.setMainChar(&mainChar);

    float tileWidth = 0.2f;
    float tileHeight = tileWidth * 0.44f;
    float tileHorizontalOffset = tileWidth * 0.64f;
    float tileVerticalOffset = tileHeight * 0.8181818181f;
    float tileAdditionalHorizontalOffsetPerRow = tileWidth * 0.36f;
    float tileAdditionalVerticalOffsetPerRow = tileHeight * 0.18181818f * -1.0f;

    MapFromImageIsometric mapFromImageIsometric {
        &textureManager,
        "maps/01.png",
        tileWidth,
        tileHeight,
        tileHorizontalOffset,
        tileVerticalOffset,
        tileAdditionalHorizontalOffsetPerRow,
        tileAdditionalVerticalOffsetPerRow
    };

    engine.setMapComponent(&mapFromImageIsometric);

    SpawnScheduleFactory spawnScheduleFactory;
    engine.setSpawnSchedule(spawnScheduleFactory.makeStage1());

    GameObject healthBar {};
    healthBar.setPosition(0.01f,0.01f);
    healthBar.setSize(0.2f,0.02f);
    healthBar.addRenderComponent(std::make_unique<UiBarRenderer>(
        textureManager.loadTexture("images/ui/ui_bar_sprites.png"),
        textureManager.loadTexture(81,83,85),
        textureManager.loadTexture(168,14,54),
        [&mainChar](){
           return mainChar.getMaxHealth();
        },
        [&mainChar](){
           return mainChar.getHealth();
        }
    ));
    engine.setHealthBar(&healthBar);

    GameObject gemValueBar {};
    gemValueBar.setPosition(0.01f,0.04f);
    gemValueBar.setSize(0.02f,0.02f);
    gemValueBar.addRenderComponent(std::make_unique<UiBarRenderer>(
        textureManager.loadTexture("images/ui/ui_bar_sprites.png"),
        textureManager.loadTexture(81,83,85),
        textureManager.loadTexture(7,102,172),
        [&mainChar](){
            return mainChar.getMaxGemValue();
        },
        [&mainChar](){
            return mainChar.getCurrentGemValue();
        }
    ));
    engine.setGemValueBar(&gemValueBar);

    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            engine.handleKeyboardAndMouseEvent(event);
        }

        engine.update();
        engine.render();

        if(engine.shouldQuit()){
            break;
        }
    }

    renderer.quit();

    return 0;
}