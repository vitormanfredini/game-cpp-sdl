#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "SDLUtils.h"
#include "BinaryResourceLoader.h"
#include "Character.h"
#include "Input.h"
#include "DeltaTime.h"
#include "Menu.h"
#include "HealthBar.h"
#include "Projectile.h"
#include "CharacterUtils.h"
#include "Renderer.h"
#include "Camera.h"
#include "MapGenerator.h"
#include "GameEngine.h"
#include "weapons/FireBallThrower.h"

#include <chrono>
#include <thread>

int main() {

    Renderer renderer {
        2*1200,
        2*800
    };

    if(!renderer.initialize()){
        return -1;
    }

    Input input;
    DeltaTime deltaTime { 60 };

    Camera camera {
        20,
        0.0f,
        0.0f
    };

    GameEngine engine {
        &renderer,
        &camera,
        &deltaTime,
        &input
    };

    Menu menu;
    menu.setTexture(renderer.loadTexture("images/menu.png"));
    menu.setPosition(0.0f,0.0f);
    menu.setSize(1.0f,1.0f);

    engine.setMenu(&menu);

    MapGenerator mapGenerator;
    mapGenerator.setGroundTexture(renderer.loadTexture("images/grass.png"));

    engine.setMapGenerator(&mapGenerator);

    Character mainChar;
    mainChar.setTexture(renderer.loadTexture("images/dog.png"));
    mainChar.setPosition(0.0f,0.0f);
    mainChar.setSize(0.10f,0.10f);
    mainChar.setVelocity(0.005f);
    mainChar.setCollisionAttack(0.03f);
    mainChar.setCollisionBox(0.10f,0.033f);

    std::unique_ptr<FireBallThrower> weaponFireDogThrower = std::make_unique<FireBallThrower>();
    weaponFireDogThrower->setProjectileTexture(renderer.loadTexture("images/projectile.png"));
    weaponFireDogThrower->setAttack(0.5f);
    weaponFireDogThrower->setFireFrequency(10);
    mainChar.addWeapon(std::move(weaponFireDogThrower));

    engine.setMainChar(&mainChar);

    LevelScript level1;
    level1.addKeyframe({ 60, 3, EnemyType::Regular });
    level1.addKeyframe({ 120, 5, EnemyType::Regular });
    level1.addKeyframe({ 300, 5, EnemyType::Regular });
    level1.addKeyframe({ 600, 4, EnemyType::Bigger });
    level1.addKeyframe({ 620, 4, EnemyType::Bigger });
    level1.addKeyframe({ 640, 5, EnemyType::Bigger });
    level1.addKeyframe({ 900, 4, EnemyType::Bigger });
    level1.addKeyframe({ 920, 4, EnemyType::Bigger });
    level1.addKeyframe({ 940, 5, EnemyType::Bigger });
    level1.addKeyframe({ 1200, 6, EnemyType::Bigger });
    level1.addKeyframe({ 1300, 6, EnemyType::Bigger });
    level1.addKeyframe({ 1400, 9, EnemyType::Bigger });
    level1.addKeyframe({ 1800, 3, EnemyType::Boss });
    level1.addKeyframe({ 2000, 5, EnemyType::Boss });

    engine.setLevelScript(&level1);

    HealthBar healthBar {
        renderer.loadTexture(200,69,49),
        renderer.loadTexture(129,147,127)
    };
    healthBar.setPosition(0.0f,0.0f);
    healthBar.setSize(1.0f,0.03f);
    healthBar.setHealth(mainChar.getHealth());
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