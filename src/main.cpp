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

int main() {

    int windowWidth = 2*1200;
    int windowHeight = 2*800;

    Renderer renderer {windowWidth, windowHeight};

    if(!renderer.initialize()){
        return -1;
    }

    std::vector<IRenderable*> renderables = {};

    Character mainChar;
    mainChar.setTexture(renderer.loadTexture("images/dog.png"));
    mainChar.setPosition(renderer.getPositionCenterOfScreen());
    mainChar.setSize(0.2f,0.2f);
    mainChar.setVelocity(0.01f);

    int numberOfEnemies = 10;
    std::vector<Character*> enemies = {};
    for(size_t c=0; c<numberOfEnemies; c++){

        enemies.push_back(new Character());
        enemies[c]->setTexture(renderer.loadTexture("images/enemy.png"));
        enemies[c]->setPosition(renderer.getRandomPositionOutsideScreen());
        enemies[c]->setSize(0.15f,0.15f);
        enemies[c]->setVelocity(0.003f);

        enemies.push_back(enemies[c]);
    }

    Menu menu;
    menu.setTexture(renderer.loadTexture("images/menu.png"));
    menu.setPosition(0.0f,0.0f);
    menu.setSize(renderer.getMaxWidth(),renderer.getMaxHeight());

    HealthBar healthBar;
    healthBar.setTexture(renderer.loadTexture(182,114,28));
    healthBar.setPosition(0,0);
    healthBar.setSize(1.0f,0.03f);
    healthBar.setMaxWidth(renderer.getMaxWidth());
    healthBar.setHealth(mainChar.getHealth());

    std::vector<Projectile*> projectiles = {};
    
    Input input;
    DeltaTime deltaTime;

    SDL_Event event;
    bool running = true;

    bool paused = false;

    while (running) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    input.handleKeyDown(event.key.keysym.sym);
                    menu.handleKeyDown(event.key.keysym.sym);
                    if(!paused && event.key.keysym.sym==SDLK_ESCAPE){
                        paused = true;
                    }
                    if(paused && event.key.keysym.sym==SDLK_RETURN){
                        paused = false;
                    }
                    break;
                case SDL_KEYUP:
                    input.handleKeyUp(event.key.keysym.sym);
                    break;
            }
        }

        deltaTime.update();
        int updatesNeeded = deltaTime.getUpdatesNeeded();

        if(!paused){

            for(int update=0;update<updatesNeeded;update++){
                
                input.update();

                mainChar.update();
                mainChar.move(input.getMovementDirection());
                
                if(mainChar.shouldFireProjectile()){
                    int closestEnemyIndex = CharacterUtils::getClosestCharacterIndex(enemies, mainChar);
                    if(closestEnemyIndex >= 0){
                        projectiles.push_back(
                            mainChar.createProjectile(enemies[closestEnemyIndex], renderer.loadTexture("images/projectile.png"))
                        );
                    }
                }

                for(Projectile* projectile : projectiles){
                    projectile->update();
                }

                std::vector<int> diedEnemies = {};

                for(size_t e=0; e<enemies.size(); e++){
                    enemies[e]->moveTowards(&mainChar);
                    if(mainChar.isCollidingWith(enemies[e])){
                        mainChar.takeDamageFrom(enemies[e]);
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
                    newEnemy->setTexture(renderer.loadTexture("images/enemy.png"));
                    newEnemy->setPosition(renderer.getRandomPositionOutsideScreen());
                    newEnemy->setSize(0.15f,0.15f);
                    newEnemy->setVelocity(0.005f);
                    enemies.push_back(newEnemy);
                }
            }

            healthBar.setHealth(mainChar.getHealth());

            if(mainChar.getHealth() <= 0.0f){
                paused = true;
            }
            
        }

        renderables.clear();
        renderables.push_back(&mainChar);
        for(size_t e=0; e<enemies.size(); e++){
            renderables.push_back(enemies[e]);
        }
        for(size_t p=0; p<projectiles.size(); p++){
            renderables.push_back(projectiles[p]);
        }
        renderables.push_back(&healthBar);
        if(paused){
            renderables.push_back(&menu);
        }

        renderer.render(renderables);

        //running = false;
    }

    

    return 0;
}