#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "SDLUtils.h"
#include "BinaryResourceLoader.h"
#include "Character.h"
#include "Input.h"
#include "DeltaTime.h"
#include "Menu.h"
#include <random>
#include "HealthBar.h"
#include "Projectile.h"

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
    SDL_Texture* texture_healthbar = SDLUtils::textureFromRGB(renderer,240,20,20);

    Character mainChar;
    mainChar.setTexture(texture_dog);
    mainChar.setPosition(220,80);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    int numberOfEnemies = 15;
    std::vector<Character> enemies = {};
    for(size_t c=0; c<numberOfEnemies; c++){
        enemies.push_back(Character{});
        enemies[c].setTexture(texture_enemy);
        enemies[c].setPosition(
            round(2.0f * 800.0f * dis(gen)),
            round(2.0f * 600.0f * dis(gen))
        );
        enemies[c].setVelocity(0.5f + 2.5f * dis(gen));
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
                mainChar.move(input.getMovementDirections(), deltaTimeInfo.multiplier);
                
                if(mainChar.shouldFireProjectile()){
                    Projectile newProjectile {};
                    newProjectile.setAttack(1.0f);
                    newProjectile.setPosition(
                        mainChar.getX() + (mainChar.getWidth() / 2.0f),
                        mainChar.getY() + (mainChar.getHeight() / 2.0f)
                    );
                    newProjectile.setTexture(texture_projectile);
                    newProjectile.setVelocity(1.0f, 0.5f);
                    projectiles.push_back(newProjectile);
                }

                for(Projectile& projectile : projectiles){
                    projectile.update();
                }

                std::vector<int> diedEnemies = {};

                for(size_t c=0; c<enemies.size(); c++){
                    enemies[c].moveTowards(mainChar, deltaTimeInfo.multiplier);
                    if(mainChar.isCollidingWith(enemies[c])){
                        mainChar.takeDamageFrom(enemies[c].getAttack());
                    }
                    for(Projectile& projectile : projectiles){
                        if(projectile.isCollidingWith(enemies[c])){
                            enemies[c].takeDamageFrom(projectile.getAttack());
                        }
                    }
                    if(enemies[c].getHealth() <= 0.0f){
                        diedEnemies.push_back(c);
                    }
                }
                for(int index : diedEnemies){
                    enemies.erase(enemies.begin() + index);
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