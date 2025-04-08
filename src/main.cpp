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
#include "GameObject/SpriteRenderer.h"
#include "GameObject/ButtonRenderer.h"
#include "TextureManager.h"
#include "GameObject/BoxCollider.h"
#include "GameObject/UiHealthBarRenderer.h"
#include "GameObject/UiGemValueBarRenderer.h"
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

    StateManager stateManager;

    GameEngine engine {
        &renderer,
        &camera,
        &deltaTime,
        &input,
        &textureManager,
        &stateManager
    };

    Menu upgradeMenu;
    upgradeMenu.setPosition(0.0f,0.0f);
    upgradeMenu.setSize(1.0f,1.0f);
    upgradeMenu.addRenderComponent(std::make_unique<SpriteRenderer>(
        textureManager.loadTexture("images/menu.png"),
        Alignment::UI
    ));

    std::unique_ptr<Button> option1Button = std::make_unique<Button>();
    option1Button->setPosition(0.33f, 0.2f);
    option1Button->setSize(0.33f, 0.166f);
    option1Button->addRenderComponent(std::make_unique<ButtonRenderer>(
        textureManager.loadTexture("images/button_upgrademenu_option.png")
    ));
    option1Button->setCallback([&stateManager]() {
        std::cout << "option 1 clicked" << std::endl;
        stateManager.setGamePlayState(GameplayState::Play);
    });
    upgradeMenu.addButton(std::move(option1Button));

    std::unique_ptr<Button> option2Button = std::make_unique<Button>();
    option2Button->setPosition(0.33f, 0.4f);
    option2Button->setSize(0.33f, 0.166f);
    option2Button->addRenderComponent(std::make_unique<ButtonRenderer>(
        textureManager.loadTexture("images/button_upgrademenu_option.png")
    ));
    option2Button->setCallback([&stateManager]() {
        std::cout << "option 2 clicked" << std::endl;
        stateManager.setGamePlayState(GameplayState::Play);
    });
    upgradeMenu.addButton(std::move(option2Button));

    std::unique_ptr<Button> option3Button = std::make_unique<Button>();
    option3Button->setPosition(0.33f, 0.6f);
    option3Button->setSize(0.33f, 0.166f);
    option3Button->addRenderComponent(std::make_unique<ButtonRenderer>(
        textureManager.loadTexture("images/button_upgrademenu_option.png")
    ));
    option3Button->setCallback([&stateManager]() {
        std::cout << "option 3 clicked" << std::endl;
        stateManager.setGamePlayState(GameplayState::Play);
    });
    upgradeMenu.addButton(std::move(option3Button));

    engine.setSubMenu(&upgradeMenu);



    Menu mainMenu;
    mainMenu.setPosition(0.0f,0.0f);
    mainMenu.setSize(1.0f,1.0f);
    mainMenu.addRenderComponent(std::make_unique<SpriteRenderer>(
        textureManager.loadTexture("images/menu.png"),
        Alignment::UI
    ));

    std::unique_ptr<Button> startButton = std::make_unique<Button>();
    startButton->setPosition(0.33f, 0.3f);
    startButton->setSize(0.33f, 0.166f);
    startButton->addRenderComponent(std::make_unique<ButtonRenderer>(
        textureManager.loadTexture("images/button_start.png")
    ));
    startButton->setCallback([&stateManager]() {
        stateManager.setMainState(MainState::Gameplay);
    });
    mainMenu.addButton(std::move(startButton));

    std::unique_ptr<Button> exitButton = std::make_unique<Button>();
    exitButton->setPosition(0.33f, 0.53f);
    exitButton->setSize(0.33f, 0.166f);
    exitButton->addRenderComponent(std::make_unique<ButtonRenderer>(
        textureManager.loadTexture("images/button_exit.png")
    ));
    exitButton->setCallback([&stateManager]() {
        stateManager.triggerQuit();
    });
    mainMenu.addButton(std::move(exitButton));

    engine.setMenu(&mainMenu);

    // RandomMap randomMap {&textureManager, 8};
    MapFromImage mapFromImage {&textureManager, "maps/01.png", 8};

    engine.setMapComponent(&mapFromImage);

    MainCharacter mainChar;
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
    mainChar.setAdvanceLevelCallback([&stateManager,&input](int level) {
        stateManager.setGamePlayState(GameplayState::UpgradeMenu);
        input.reset();
    });

    std::unique_ptr<FireBallThrower> weaponFireDogThrower = std::make_unique<FireBallThrower>();
    weaponFireDogThrower->setProjectileTexture(textureManager.loadTexture("images/projectile.png"));
    weaponFireDogThrower->setAttack(0.5f);
    weaponFireDogThrower->setFireFrequency(10);
    mainChar.addWeapon(std::move(weaponFireDogThrower));

    engine.setMainChar(&mainChar);

    LevelScript level1;
    level1.addKeyframe({ 60, 3, CharacterType::Regular });
    level1.addKeyframe({ 90, 6, CharacterType::Regular });
    level1.addKeyframe({ 120, 5, CharacterType::Regular });
    level1.addKeyframe({ 160, 10, CharacterType::Regular });
    level1.addKeyframe({ 300, 15, CharacterType::Regular });
    level1.addKeyframe({ 420, 15, CharacterType::Regular });
    level1.addKeyframe({ 600, 14, CharacterType::Bigger });
    level1.addKeyframe({ 620, 14, CharacterType::Bigger });
    level1.addKeyframe({ 640, 15, CharacterType::Bigger });
    level1.addKeyframe({ 750, 14, CharacterType::Bigger });
    level1.addKeyframe({ 900, 14, CharacterType::Bigger });
    level1.addKeyframe({ 920, 14, CharacterType::Bigger });
    level1.addKeyframe({ 940, 15, CharacterType::Bigger });
    level1.addKeyframe({ 1200, 16, CharacterType::Bigger });
    level1.addKeyframe({ 1300, 16, CharacterType::Bigger });
    level1.addKeyframe({ 1400, 19, CharacterType::Bigger });
    level1.addKeyframe({ 1800, 13, CharacterType::Boss });
    level1.addKeyframe({ 2000, 15, CharacterType::Boss });
    level1.addKeyframe({ 4000, 30, CharacterType::Boss });
    level1.addKeyframe({ 6000, 30, CharacterType::Boss });

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

    GameObject gemValueBar {};
    gemValueBar.setPosition(0.0f,0.03f);
    gemValueBar.setSize(1.0f,0.03f);
    gemValueBar.addRenderComponent(std::make_unique<UiGemValueBarRenderer>(
        textureManager.loadTexture(129,147,127),
        textureManager.loadTexture(69,200,49),
        &mainChar
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

    return 0;
}