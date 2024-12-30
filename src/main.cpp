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

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("GAME!!!!!1", 100, 100, 2*800, 2*600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture_dog = BinaryResourceLoader::toTexture(renderer, "images/dog.png");
    SDL_Texture* texture_enemy = BinaryResourceLoader::toTexture(renderer, "images/enemy.png");
    SDL_Texture* texture_menu = BinaryResourceLoader::toTexture(renderer, "images/menu.png");
    SDL_Texture* texture_projectile = BinaryResourceLoader::toTexture(renderer, "images/projectile.png");
    SDL_Texture* texture_healthbar = SDLUtils::textureFromRGB(renderer,182,114,28);

    Character mainChar;
    mainChar.setTexture(texture_dog);
    mainChar.setPosition(220,80);

    int numberOfEnemies = 15;
    std::vector<Character> enemies = {};
    for(size_t c=0; c<numberOfEnemies; c++){
        enemies.push_back(Character{});
        enemies[c].setTexture(texture_enemy);
        std::vector<float> pos = CharacterUtils::getRandomPositionOutsideScreen();
        enemies[c].setPosition(
            pos[0],
            pos[1]
        );
        enemies[c].setVelocity(2.0f);
    }

    Menu menu;
    menu.setTexture(texture_menu);

    HealthBar healthBar;
    healthBar.setTexture(texture_healthbar);
    healthBar.setPosition(0,0);

    std::vector<Projectile> projectiles = {};
    
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
        auto deltaTimeInfo = deltaTime.getDeltaTimeInfo();

        if(!paused){

            for(int update=0;update<deltaTimeInfo.updatesNeeded;update++){
                
                input.update();

                mainChar.update();
                mainChar.move(input.getMovementDirection(), deltaTimeInfo.multiplier);
                
                if(mainChar.shouldFireProjectile()){
                    int closestEnemyIndex = CharacterUtils::getClosestCharacterIndex(enemies, mainChar);
                    if(closestEnemyIndex >= 0){
                        projectiles.push_back(
                            mainChar.createProjectile(enemies[closestEnemyIndex], texture_projectile)
                        );
                    }
                }

                for(Projectile& projectile : projectiles){
                    projectile.update();
                }

                std::vector<int> diedEnemies = {};

                for(size_t e=0; e<enemies.size(); e++){
                    enemies[e].moveTowards(mainChar, deltaTimeInfo.multiplier);
                    if(mainChar.isCollidingWith(enemies[e])){
                        mainChar.takeDamageFrom(enemies[e]);
                    }
                    std::vector<int> diedProjectiles = {};
                    for(size_t p=0; p<projectiles.size(); p++){
                        if(projectiles[p].isCollidingWith(enemies[e])){
                            enemies[e].takeDamageFrom(projectiles[p]);
                        }
                        if(projectiles[p].remainingHits() <= 0){
                            diedProjectiles.push_back(p);
                        }
                    }
                    for(int index : diedProjectiles){
                        projectiles.erase(projectiles.begin() + index);
                    }
                    if(enemies[e].getHealth() <= 0.0f){
                        diedEnemies.push_back(e);
                    }
                }
                for(int index : diedEnemies){
                    enemies.erase(enemies.begin() + index);

                    enemies.push_back(CharacterUtils::createNewCharacter(texture_enemy, 2.0f));
                    enemies.push_back(CharacterUtils::createNewCharacter(texture_enemy, 2.0f));
                }
            }

            healthBar.setHealth(mainChar.getHealth());

            if(mainChar.getHealth() <= 0.0f){
                paused = true;
            }
            
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        mainChar.draw(renderer);
        for(Character& enemy : enemies){
            enemy.draw(renderer);
        }
        healthBar.draw(renderer);
        for(Projectile& projectile : projectiles){
            projectile.draw(renderer);
        }
        if(paused){
            menu.draw(renderer);
        }

        SDL_RenderPresent(renderer);

        //running = false;
    }

    SDL_DestroyTexture(texture_dog);
    SDL_DestroyTexture(texture_enemy);
    SDL_DestroyTexture(texture_menu);
    SDL_DestroyTexture(texture_projectile);
    SDL_DestroyTexture(texture_healthbar);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}