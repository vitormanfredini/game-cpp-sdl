#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "SDLUtils.h"
#include "BinaryResourceLoader.h"
#include "Input.h"
#include "DeltaTime.h"
#include "Menu.h"
#include "Weapons/Projectile.h"
#include "GameObject/Character/Character.h"
#include "Renderer.h"
#include "Camera.h"
#include "Maps/MapComponent.h"
#include "GameEngine.h"
#include "Weapons/FireBallThrower.h"
#include "GameObject/SpriteRenderer.h"
#include "TextureManager.h"
#include "GameObject/BoxCollider.h"
#include "GameObject/UiHealthBarRenderer.h"
#include "Maps/MapFromImage.h"
#include "Maps/RandomMap.h"

int main() {

    Renderer renderer {
        2*1200,
        2*800
    };

    if(!renderer.initializedCorrectly()){
        return -1;
    }

    Input input;
    DeltaTime deltaTime { 60 };

    Camera camera {
        20,
        0.0f,
        0.0f
    };

    TextureManager textureManager {renderer.getSDLRenderer()};

    GameEngine engine {
        &renderer,
        &camera,
        &deltaTime,
        &input,
        &textureManager
    };

    Menu menu;
    menu.setPosition(0.0f,0.0f);
    menu.setSize(1.0f,1.0f);
    menu.addRenderComponent(std::make_unique<SpriteRenderer>(
        textureManager.loadTexture("images/menu.png"),
        Alignment::UI
    ));

    engine.setMenu(&menu);

    // RandomMap randomMap {&textureManager, 8};
    MapFromImage mapFromImage {&textureManager, "maps/01.png", 10};

    engine.setMapComponent(&mapFromImage);

    Character mainChar;
    mainChar.setPosition(0.0f,0.0f);
    mainChar.setSize(0.10f,0.10f);
    mainChar.setVelocity(0.005f);
    mainChar.setInitialHealth(1.0f);
    mainChar.setCollisionAttack(0.03f);
    mainChar.addRenderComponent(std::make_unique<SpriteRenderer>(
        textureManager.loadTexture("images/dog.png"),
        Alignment::BottomUpCentered
    ));
    mainChar.setCollisionComponent(std::make_unique<BoxCollider>(
        0.10f,
        0.033f,
        Alignment::Centered
    ));

    std::unique_ptr<FireBallThrower> weaponFireDogThrower = std::make_unique<FireBallThrower>();
    weaponFireDogThrower->setProjectileTexture(textureManager.loadTexture("images/projectile.png"));
    weaponFireDogThrower->setAttack(0.5f);
    weaponFireDogThrower->setFireFrequency(10);
    mainChar.addWeapon(std::move(weaponFireDogThrower));

    engine.setMainChar(&mainChar);

    LevelScript level1;
    level1.addKeyframe({ 60, 3, CharacterType::Regular });
    level1.addKeyframe({ 120, 5, CharacterType::Regular });
    level1.addKeyframe({ 300, 15, CharacterType::Regular });
    level1.addKeyframe({ 600, 14, CharacterType::Bigger });
    level1.addKeyframe({ 620, 14, CharacterType::Bigger });
    level1.addKeyframe({ 640, 15, CharacterType::Bigger });
    level1.addKeyframe({ 900, 14, CharacterType::Bigger });
    level1.addKeyframe({ 920, 14, CharacterType::Bigger });
    level1.addKeyframe({ 940, 15, CharacterType::Bigger });
    level1.addKeyframe({ 1200, 16, CharacterType::Bigger });
    level1.addKeyframe({ 1300, 16, CharacterType::Bigger });
    level1.addKeyframe({ 1400, 19, CharacterType::Bigger });
    level1.addKeyframe({ 1800, 13, CharacterType::Boss });
    level1.addKeyframe({ 2000, 15, CharacterType::Boss });

    engine.setLevelScript(&level1);

    GameObject healthBar {};
    healthBar.setPosition(0.0f,0.0f);
    healthBar.setSize(1.0f,0.03f);
    healthBar.addRenderComponent(std::make_unique<UiHealthBarRenderer>(
        textureManager.loadTexture(129,147,127),
        textureManager.loadTexture(200,69,49),
        &mainChar
    ));
    engine.setHealthBar(&healthBar);

    SDL_Event event;

    while (true) {

        bool paused = engine.isPaused();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    engine.triggerQuit();
                    break;
                case SDL_KEYDOWN:
                    input.handleKeyDown(event.key.keysym.sym);
                    menu.handleKeyDown(event.key.keysym.sym);
                    if(event.key.keysym.sym==SDLK_ESCAPE){
                        if(engine.isPaused()){
                            engine.triggerQuit();
                        }
                        engine.setPause(true);
                    }
                    if(paused && event.key.keysym.sym==SDLK_RETURN){
                        engine.setPause(false);
                    }
                    break;
                case SDL_KEYUP:
                    input.handleKeyUp(event.key.keysym.sym);
                    break;
            }
        }

        engine.update();
        engine.render();

        if(engine.shouldQuit()){
            break;
        }
    }

    return 0;
}