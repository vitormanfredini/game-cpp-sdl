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
#include "GameObject/UiHealthBarRenderer.h"
#include "GameObject/UiGemValueBarRenderer.h"
#include "Maps/MapFromImage.h"
#include "GameObject/Ui/MenuFactory.h"
#include "Font/FontManager.h"
#include "AudioManager.h"
#include "LevelSongPlayer.h"

int main() {

    SDL_Window* window = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;
    int widthPixels = 1200 * 2;
    int heightPixels = 800 * 2;

    SDLUtils::initializeSDL(window, sdl_renderer, widthPixels, heightPixels);

    Renderer renderer {
        sdl_renderer,
        widthPixels,
        heightPixels
    };

    Input input;
    DeltaTime deltaTime { 60 };

    Camera camera {
        20,
        0.0f,
        0.0f
    };

    AudioManager audioManager;
    FontManager fontManager;
    TextureManager textureManager {renderer.getSDLRenderer(), &fontManager};
    StateManager stateManager;
    ItemFactory itemFactory(&textureManager);
    UpgradeFactory upgradeFactory(&itemFactory);
    MenuFactory menuFactory(&textureManager, &stateManager);

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
        &audioManager
    };

    std::unique_ptr<LevelManager> mainCharLevelManager = std::make_unique<LevelManager>();
    mainCharLevelManager->setOnAdvanceLevelCallback([&engine](int level) {
        engine.onAdvanceLevel(level);
    });

    Character mainChar;
    mainChar.setPosition(0.0f,0.0f);
    mainChar.setSize(0.10f,0.10f);
    mainChar.setInitialBaseSpeed(0.5f);
    mainChar.setInitialMaxHealth(1.0f);
    mainChar.setCollisionAttack(0.03f);
    mainChar.setCollisionSound(audioManager.loadAudio("audio/collision.wav"));
    mainChar.addRenderComponent(std::make_unique<SpriteRenderer>(
        textureManager.loadTexture("images/dog.png"),
        Alignment::BottomUpCentered
    ));
    mainChar.setCollisionComponent(std::make_unique<BoxCollider>(
        0.10f,
        0.033f,
        Alignment::Centered
    ));
    mainChar.setLevelManager(std::move(mainCharLevelManager));
    

    std::unique_ptr<FireBallThrower> weaponFireDogThrower = std::make_unique<FireBallThrower>();
    weaponFireDogThrower->setProjectileTexture(textureManager.loadTexture("images/projectile.png"));
    weaponFireDogThrower->setAttack(0.5f);
    weaponFireDogThrower->setFireFrequency(20);
    weaponFireDogThrower->setFireSound(audioManager.loadAudio("audio/pew.wav"));
    mainChar.addWeapon(std::move(weaponFireDogThrower));

    std::unique_ptr<Sword> weaponSword = std::make_unique<Sword>();
    weaponSword->setProjectileTexture(textureManager.loadTexture("images/projectile.png"));
    weaponSword->setAttack(2.0f);
    weaponSword->setFireFrequency(87);
    weaponSword->setFireSound(audioManager.loadAudio("audio/fu.wav"));
    mainChar.addWeapon(std::move(weaponSword));

    

    engine.setMainChar(&mainChar);

    MapFromImage mapFromImage {&textureManager, "maps/01.png", 8};

    engine.setMapComponent(&mapFromImage);

    LevelScript level1;

    for(int c=0;c<100;c++){
        int framesOffset = c * 680;
        level1.addKeyframe({ framesOffset + 240, 1 * (c+1), CharacterType::Regular });
        level1.addKeyframe({ framesOffset + 540, 3 * (c+1), CharacterType::Regular });
        level1.addKeyframe({ framesOffset + 1000, 3 * (c+1), CharacterType::Regular });
        level1.addKeyframe({ framesOffset + 1500, 5 * (c+1), CharacterType::Regular });

        level1.addKeyframe({ framesOffset + 2300, 5 * (c+1), CharacterType::Regular });
        level1.addKeyframe({ framesOffset + 2400, 1 * (c+1), CharacterType::Bigger });
        level1.addKeyframe({ framesOffset + 2500, 2 * (c+1), CharacterType::Regular });

        level1.addKeyframe({ framesOffset + 3500, 5 * (c+1), CharacterType::Regular });
        level1.addKeyframe({ framesOffset + 3600, 3 * (c+1), CharacterType::Bigger });
        level1.addKeyframe({ framesOffset + 3700, 5 * (c+1), CharacterType::Regular });

        level1.addKeyframe({ framesOffset + 4150, 1 * (c+1), CharacterType::Boss });

        level1.addKeyframe({ framesOffset + 4600, 30 * (c+1), CharacterType::Regular });

        level1.addKeyframe({ framesOffset + 5100, 20 * (c+1), CharacterType::Bigger });

        level1.addKeyframe({ framesOffset + 5100, 1 * (c+1), CharacterType::Boss });

        level1.addKeyframe({ framesOffset + 5500, 15 * (c+1), CharacterType::Regular });
        level1.addKeyframe({ framesOffset + 5500, 20 * (c+1), CharacterType::Bigger });

        level1.addKeyframe({ framesOffset + 6000, 3 * (c+1), CharacterType::Boss });

        level1.addKeyframe({ framesOffset + 6700, 10 * (c+1), CharacterType::Regular });
        level1.addKeyframe({ framesOffset + 6700, 10 * (c+1), CharacterType::Bigger });
        level1.addKeyframe({ framesOffset + 6700, 10 * (c+1), CharacterType::Boss });
    }


    LevelSongPlayer song1 {&audioManager, 240};
    song1.addLevelLoops({
        audioManager.loadAudio("audio/song1/layer1.wav")
    });
    song1.addLevelLoops({
        audioManager.loadAudio("audio/song1/layer1.wav"),
        audioManager.loadAudio("audio/song1/layer2.wav")
    });
    song1.addLevelLoops({
        audioManager.loadAudio("audio/song1/layer1.wav"),
        audioManager.loadAudio("audio/song1/layer2.wav"),
        audioManager.loadAudio("audio/song1/layer3.wav")
    });
    song1.addLevelLoops({
        audioManager.loadAudio("audio/song1/layer1.wav"),
        audioManager.loadAudio("audio/song1/layer2.wav"),
        audioManager.loadAudio("audio/song1/layer3.wav"),
        audioManager.loadAudio("audio/song1/layer4.wav")
    });
    song1.addLevelLoops({
        audioManager.loadAudio("audio/song1/layer1.wav"),
        audioManager.loadAudio("audio/song1/layer2.wav"),
        audioManager.loadAudio("audio/song1/layer3.wav"),
        audioManager.loadAudio("audio/song1/layer5.wav")
    });

    engine.setLevelScript(&level1);

    engine.setSongPlayer(&song1);

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

    SDLUtils::quitSDL(window, sdl_renderer);

    return 0;
}