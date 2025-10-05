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

        SDL_Color numbersTextColor = { 58, 58, 50, 255 };
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
                &numbersTextColor,
                TextRenderMethod::Centered
            ),
            textureManager->drawTextOnTexture(
                textureManager->loadTexture(0,0,0,0,numberWidth,numberHeight),
                "1",
                FontStyle::MainMenu,
                &numbersTextColor,
                TextRenderMethod::Centered
            ),
            Alignment::UI
        ));

        int buttonHeight = 40;
        int buttonWidth = 190;

        SDL_Color mainMenuButtonTextColorIdle = { 255, 255, 255, 255 };
        SDL_Color mainMenuButtonTextColorHover = { 29, 158, 20, 255 };
        SDL_Color mainMenuButtonTextColorPressed = { 29, 158, 20, 255 };

        std::unique_ptr<Button> startButton = std::make_unique<Button>();
        startButton->setPosition(0.106f, 0.506f);
        startButton->setSize(0.147f, 0.0305f);
        startButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->createButtonTexture(
                buttonWidth,
                buttonHeight,
                "new game",
                FontStyle::MainMenu,
                mainMenuButtonTextColorIdle,
                mainMenuButtonTextColorHover,
                mainMenuButtonTextColorPressed,
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight),
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight),
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight)
            )
        ));
        startButton->setCallback([stateManager]() {
            stateManager->setMainState(MainState::Gameplay);
        });
        prototypes[MenuType::MainMenu]->addButton(std::move(startButton));

        std::unique_ptr<Button> optionsButton = std::make_unique<Button>();
        optionsButton->setPosition(0.106f, 0.532f);
        optionsButton->setSize(0.147f, 0.0305f);
        optionsButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->createButtonTexture(
                buttonWidth,
                buttonHeight,
                "options",
                FontStyle::MainMenu,
                mainMenuButtonTextColorIdle,
                mainMenuButtonTextColorHover,
                mainMenuButtonTextColorPressed,
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight),
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight),
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight)
            )
        ));
        optionsButton->setCallback([stateManager]() {
            //
        });
        prototypes[MenuType::MainMenu]->addButton(std::move(optionsButton));

        std::unique_ptr<Button> exitButton = std::make_unique<Button>();
        exitButton->setPosition(0.106f, 0.584f);
        exitButton->setSize(0.147f, 0.0305f);
        exitButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->createButtonTexture(
                buttonWidth,
                buttonHeight,
                "exit",
                FontStyle::MainMenu,
                mainMenuButtonTextColorIdle,
                mainMenuButtonTextColorHover,
                mainMenuButtonTextColorPressed,
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight),
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight),
                textureManager->loadTexture(0,0,0,255,buttonWidth,buttonHeight)
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
            textureManager->loadTexture("images/menu.png"),
            Alignment::UI
        ));

    }

    std::unique_ptr<Menu> createMainMenu() {
        return prototypes[MenuType::MainMenu]->clone();
    }

    std::unique_ptr<Menu> createUpgradeMenu(Character* charReceivesReward, UpgradeFactory* upgradeFactory) {
        std::unique_ptr<Menu> upgradeMenu = prototypes[MenuType::UpgradeMenu]->clone();
        
        std::vector<std::unique_ptr<UpgradeOption>> options = upgradeFactory->createRandomUpgradeOptions(3);

        if(options.size() == 0){
            std::cerr << "options.size() == 0" << std::endl;
            // provavelmente acabaram os upgrades. implementar itens no submenu para colocar no lugar
        }

        SDL_Color upgradeMenuButtonTextColor = { 155, 198, 150, 255 };

        for(size_t c=0; c<options.size(); c++){
            std::shared_ptr<UpgradeOption> upgradeOption = std::move(options[c]);

            std::unique_ptr<Button> optionButton = std::make_unique<Button>();
            optionButton->setPosition(0.166f, 0.2f + (c * 0.2f));
            optionButton->setSize(0.66f, 0.166f);
            optionButton->addRenderComponent(std::make_unique<ButtonRenderer>(
                textureManager->drawTextOnTexture(
                    textureManager->loadTexture("images/upgrademenu_button_base.png"),
                    upgradeOption->description.c_str(),
                    FontStyle::UpgradeMenu,
                    &upgradeMenuButtonTextColor,
                    TextRenderMethod::ButtonCentered
                )
            ));
            
            optionButton->setCallback([this, charReceivesReward, upgradeOption, upgradeFactory]() {
                std::unique_ptr<UpgradeComponent> upgrade = upgradeFactory->redeemUpgrade(upgradeOption.get());
                if(upgrade->getType() == UpgradeComponent::Type::Stat){
                    charReceivesReward->consumeStatUpgrade(upgrade->getStatUpgrade());
                }else if(upgrade->getType() == UpgradeComponent::Type::Item){
                    charReceivesReward->consumeItem(upgrade->getItem());
                }else if(upgrade->getType() == UpgradeComponent::Type::Weapon){
                    charReceivesReward->consumeWeaponUpgrade(upgrade->getWeaponUpgrade());
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
