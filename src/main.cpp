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

    int updatesPerSecond = 120;

    Input input;
    DeltaTime deltaTime { updatesPerSecond };

    Camera camera {
        static_cast<int>(std::round(static_cast<float>(updatesPerSecond) * 0.333f)),
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
    menu.setRenderAnchor(RenderAnchor::UI_FULLWIDTH_TOP);

    engine.setMenu(&menu);

    MapGenerator mapGenerator;
    mapGenerator.setGroundTexture(renderer.loadTexture("images/grass.png"));

    engine.setMapGenerator(&mapGenerator);

    Character mainChar;
    mainChar.setTexture(renderer.loadTexture("images/dog.png"));
    mainChar.setPosition(0.0f,0.0f);
    mainChar.setSize(0.10f,0.10f);
    mainChar.setVelocity(0.01f);

    engine.setMainChar(&mainChar);

    engine.addEnemies(50);

    HealthBar healthBar;
    healthBar.setTexture(renderer.loadTexture(182,114,28));
    healthBar.setPosition(0.0f,0.0f);
    healthBar.setSize(1.0f,0.03f);
    healthBar.setHealth(mainChar.getHealth());
    healthBar.setRenderAnchor(RenderAnchor::UI_FULLWIDTH_TOP);

    SDL_Event event;

    while (true) {

        bool paused = engine.isPaused();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
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