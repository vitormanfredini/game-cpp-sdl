#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "GameObject/Character/Character.h"
#include "Upgrade/UpgradeFactory.h"
#include "Upgrade/UpgradeOption.h"
#include "GameObject/Ui/Menu.h"
#include "GameObject/Ui/MenuType.h"
#include "GameObject/Ui/Button.h"
#include "GameObject/Alignment.h"
#include "GameObject/SpriteRenderer.h"
#include "GameObject/ButtonRenderer.h"
#include "StateManager/StateManager.h"
#include "TextureManager.h"
#include "GameObject/MainMenuBackgroundRenderer.h"

class MenuFactory {

public:

    MenuFactory(TextureManager* textureManager, StateManager* stateManager): textureManager(textureManager), stateManager(stateManager) {

        // Main Menu
        prototypes[MenuType::MainMenu] = std::make_unique<Menu>();

        SDL_Color whiteTextColor = { 255, 255, 255, 255 };

        SDL_Color numbersTextColor = { 148, 148, 129, 255 };
        int numberWidth = 19;
        int numberHeight = 37;

        prototypes[MenuType::MainMenu]->setPosition(0.0f,0.0f);
        prototypes[MenuType::MainMenu]->setSize(1.0f,1.0f);
        prototypes[MenuType::MainMenu]->addRenderComponent(std::make_unique<MainMenuBackgroundRenderer>(
            textureManager->loadTexture(0,0,0),
            textureManager->drawTextOnTexture(
                textureManager->loadTexture(0,0,0,0,numberWidth,numberHeight),
                "0",
                FontStyle::MainMenu,
                numbersTextColor,
                TextRenderMethod::Centered
            ),
            textureManager->drawTextOnTexture(
                textureManager->loadTexture(0,0,0,0,numberWidth,numberHeight),
                "1",
                FontStyle::MainMenu,
                numbersTextColor,
                TextRenderMethod::Centered
            ),
            textureManager->drawTextOnTexture(
                textureManager->loadTexture(0,0,0,0,551,37),
                "rage against the bad machines",
                FontStyle::MainMenu,
                whiteTextColor,
                TextRenderMethod::Centered
            ),
            Alignment::UI
        ));

        int buttonHeightPixels = 37;
        float buttonHeight = 0.0289f;
        float buttonPositionX = 0.105f;

        SDL_Color mainMenuButtonTextColorIdle = { 255, 255, 255, 255 };
        SDL_Color mainMenuButtonTextColorHover = { 29, 158, 20, 255 };
        SDL_Color mainMenuButtonTextColorPressed = { 29, 158, 20, 255 };

        std::unique_ptr<Button> startButton = std::make_unique<Button>();
        float button1WidthRatio = 4.109;
        int button1WidthPixels = std::round(static_cast<float>(buttonHeightPixels) * button1WidthRatio);
        startButton->setPosition(buttonPositionX, 0.4865f);
        startButton->setSize(buttonHeight * button1WidthRatio, buttonHeight);
        startButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->createButtonTexture(
                button1WidthPixels,
                buttonHeightPixels,
                "new game",
                FontStyle::MainMenu,
                mainMenuButtonTextColorIdle,
                mainMenuButtonTextColorHover,
                mainMenuButtonTextColorPressed,
                textureManager->loadTexture(0,0,0,255,button1WidthPixels,buttonHeightPixels),
                textureManager->loadTexture(0,0,0,255,button1WidthPixels,buttonHeightPixels),
                textureManager->loadTexture(0,0,0,255,button1WidthPixels,buttonHeightPixels)
            )
        ));
        startButton->setCallback([stateManager]() {
            stateManager->setMainState(MainState::Gameplay);
        });
        prototypes[MenuType::MainMenu]->addButton(std::move(startButton));

        std::unique_ptr<Button> optionsButton = std::make_unique<Button>();
        float button2WidthRatio = 3.594;
        int button2WidthPixels = std::round(static_cast<float>(buttonHeightPixels) * button2WidthRatio);
        optionsButton->setPosition(buttonPositionX, 0.5155f);
        optionsButton->setSize(buttonHeight * button2WidthRatio, buttonHeight);
        optionsButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->createButtonTexture(
                button2WidthPixels,
                buttonHeightPixels,
                "options",
                FontStyle::MainMenu,
                mainMenuButtonTextColorIdle,
                mainMenuButtonTextColorHover,
                mainMenuButtonTextColorPressed,
                textureManager->loadTexture(0,0,0,255,button2WidthPixels,buttonHeightPixels),
                textureManager->loadTexture(0,0,0,255,button2WidthPixels,buttonHeightPixels),
                textureManager->loadTexture(0,0,0,255,button2WidthPixels,buttonHeightPixels)
            )
        ));
        optionsButton->setCallback([stateManager]() {
            //
        });
        prototypes[MenuType::MainMenu]->addButton(std::move(optionsButton));

        std::unique_ptr<Button> exitButton = std::make_unique<Button>();
        float button3WidthRatio = 2.054;
        int button3WidthPixels = std::round(static_cast<float>(buttonHeightPixels) * button3WidthRatio);
        exitButton->setPosition(buttonPositionX, 0.5735f);
        exitButton->setSize(buttonHeight * button3WidthRatio, buttonHeight);
        exitButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->createButtonTexture(
                button3WidthPixels,
                buttonHeightPixels,
                "exit",
                FontStyle::MainMenu,
                mainMenuButtonTextColorIdle,
                mainMenuButtonTextColorHover,
                mainMenuButtonTextColorPressed,
                textureManager->loadTexture(0,0,0,255,button3WidthPixels,buttonHeightPixels),
                textureManager->loadTexture(0,0,0,255,button3WidthPixels,buttonHeightPixels),
                textureManager->loadTexture(0,0,0,255,button3WidthPixels,buttonHeightPixels)
            )
        ));
        exitButton->setCallback([stateManager]() {
            stateManager->triggerQuit();
        });
        prototypes[MenuType::MainMenu]->addButton(std::move(exitButton));

        // Upgrade Menu
        prototypes[MenuType::UpgradeMenu] = std::make_unique<Menu>();
        prototypes[MenuType::UpgradeMenu]->setPosition(0.0f,0.0f);
        prototypes[MenuType::UpgradeMenu]->setSize(1.0f,1.0f);
        prototypes[MenuType::UpgradeMenu]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture(22,33,21),
            Alignment::UI
        ));

    }

    std::unique_ptr<Menu> createMainMenu() {
        return prototypes[MenuType::MainMenu]->clone();
    }

    std::unique_ptr<Menu> createUpgradeMenu(Character* charReceivesReward, UpgradeFactory* upgradeFactory) {
        std::unique_ptr<Menu> upgradeMenu = prototypes[MenuType::UpgradeMenu]->clone();

        std::vector<UpgradeOption> options = upgradeFactory->createRandomUpgradeOptions(
            3,
            charReceivesReward->getWeaponIds()
        );

        if(options.size() == 0){
            std::cerr << "options.size() == 0" << std::endl;
            // provavelmente acabaram os upgrades. implementar itens no upgrade menu para colocar no lugar
        }

        SDL_Color upgradeMenuButtonTextColor = { 191, 191, 173, 255 };
        SDL_Color upgradeMenuButtonHoverTextColor = { 255, 255, 255, 255 };

        for(size_t c=0; c<options.size(); c++){
            UpgradeOption upgradeOption = options[c];

            float buttonHeightRatio = 0.259f;

            std::unique_ptr<Button> optionButton = std::make_unique<Button>();
            optionButton->setPosition(0.166f, 0.2f + (c * 0.2f));
            optionButton->setSize(0.66f, 0.66f*buttonHeightRatio);
            optionButton->addRenderComponent(std::make_unique<ButtonRenderer>(
                textureManager->createButtonTexture(
                    348,
                    348*buttonHeightRatio,
                    upgradeOption.description,
                    FontStyle::UpgradeMenu,
                    upgradeMenuButtonTextColor,
                    upgradeMenuButtonHoverTextColor,
                    upgradeMenuButtonHoverTextColor,
                    textureManager->loadTexture(22,33,21,255,348,348*buttonHeightRatio),
                    textureManager->loadTexture(22,33,21,255,348,348*buttonHeightRatio),
                    textureManager->loadTexture(22,33,21,255,348,348*buttonHeightRatio)
                )
            ));
            
            optionButton->setCallback([this, charReceivesReward, upgradeOption, upgradeFactory]() {
                std::unique_ptr<UpgradeComponent> upgrade = upgradeFactory->redeemUpgrade(upgradeOption);
                if(upgrade->getType() == UpgradeComponent::Type::Stat){
                    charReceivesReward->consumeStatUpgrade(upgrade->getStatUpgrade());
                }else if(upgrade->getType() == UpgradeComponent::Type::Item){
                    charReceivesReward->consumeItem(upgrade->getItem());
                }else if(upgrade->getType() == UpgradeComponent::Type::WeaponUpgrade){
                    charReceivesReward->consumeWeaponUpgrade(upgrade->getWeaponUpgrade());
                }else if(upgrade->getType() == UpgradeComponent::Type::Weapon){
                    std::unique_ptr<WeaponComponent> newWeapon = upgrade->getWeaponComponent();
                    upgradeFactory->enableUpgradesForWeapon(newWeapon->getId());
                    charReceivesReward->addWeapon(std::move(newWeapon));
                }else{
                    std::cerr << "optionButton->setCallback(): upgrade type not implemented" << std::endl;
                }
                
                stateManager->setGamePlayState(GameplayState::Play);
            });
            upgradeMenu->addButton(std::move(optionButton));
        }
        
        return upgradeMenu;
    }
    

private:
    std::unordered_map<MenuType,std::unique_ptr<Menu>> prototypes;
    TextureManager* textureManager = nullptr;
    StateManager* stateManager = nullptr;

};
