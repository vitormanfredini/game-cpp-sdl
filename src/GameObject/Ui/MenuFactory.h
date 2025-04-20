#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <random>
#include "GameObject/Character/Character.h"
#include "GameObject/UpgradeFactory.h"
#include "GameObject/Ui/Menu.h"
#include "GameObject/Ui/MenuType.h"
#include "GameObject/Ui/Button.h"
#include "GameObject/Alignment.h"
#include "GameObject/SpriteRenderer.h"
#include "GameObject/ButtonRenderer.h"
#include "StateManager/StateManager.h"
#include "TextureManager.h"

class MenuFactory {

public:

    MenuFactory(std::unique_ptr<UpgradeFactory> upgradeFactory, TextureManager* textureManager, StateManager* stateManager): upgradeFactory(std::move(upgradeFactory)), textureManager(textureManager), stateManager(stateManager) {

        // Main Menu
        prototypes[MenuType::MainMenu] = std::make_unique<Menu>();

        prototypes[MenuType::MainMenu]->setPosition(0.0f,0.0f);
        prototypes[MenuType::MainMenu]->setSize(1.0f,1.0f);
        prototypes[MenuType::MainMenu]->addRenderComponent(std::make_unique<SpriteRenderer>(
            textureManager->loadTexture("images/menu.png"),
            Alignment::UI
        ));

        SDL_Color mainMenuButtonTextColor = { 224, 210, 128, 255 };

        std::unique_ptr<Button> startButton = std::make_unique<Button>();
        startButton->setPosition(0.33f, 0.3f);
        startButton->setSize(0.33f, 0.166f);
        startButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->drawTextOnTexture(
                textureManager->loadTexture("images/menu_button_base.png"),
                "start",
                FontStyle::MainMenu,
                &mainMenuButtonTextColor,
                TextRenderMethod::ButtonCentered
            )
        ));
        startButton->setCallback([stateManager]() {
            stateManager->setMainState(MainState::Gameplay);
        });
        prototypes[MenuType::MainMenu]->addButton(std::move(startButton));

        std::unique_ptr<Button> exitButton = std::make_unique<Button>();
        exitButton->setPosition(0.33f, 0.53f);
        exitButton->setSize(0.33f, 0.166f);
        exitButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->drawTextOnTexture(
                textureManager->loadTexture("images/menu_button_base.png"),
                "exit",
                FontStyle::MainMenu,
                &mainMenuButtonTextColor,
                TextRenderMethod::ButtonCentered
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

    std::unique_ptr<Menu> createUpgradeMenu(Character* charReceivesReward) {
        std::unique_ptr<Menu> upgradeMenu = prototypes[MenuType::UpgradeMenu]->clone();
        
        std::vector<std::unique_ptr<UpgradeComponent>> upgradesToChoose = upgradeFactory->createRandomUpgrades(3);

        if(upgradesToChoose.size() == 0){
            std::cout << "upgradesToChoose.size() == 0" << std::endl;
            // provavelmente acabaram os upgrades. implementar itens no submenu para colocar no lugar
        }

        SDL_Color upgradeMenuButtonTextColor = { 155, 198, 150, 255 };

        for(size_t c=0; c<upgradesToChoose.size(); c++){
            std::shared_ptr<UpgradeComponent> upgradeOption = std::move(upgradesToChoose[c]);

            std::unique_ptr<Button> optionButton = std::make_unique<Button>();
            optionButton->setPosition(0.166f, 0.2f + (c * 0.2f));
            optionButton->setSize(0.66f, 0.166f);
            optionButton->addRenderComponent(std::make_unique<ButtonRenderer>(
                textureManager->drawTextOnTexture(
                    textureManager->loadTexture("images/upgrademenu_button_base.png"),
                    upgradeOption->getDescription().c_str(),
                    FontStyle::UpgradeMenu,
                    &upgradeMenuButtonTextColor,
                    TextRenderMethod::ButtonCentered
                )
            ));
            
            optionButton->setCallback([this, charReceivesReward, upgradeOption]() {
                upgradeFactory->playerChoseThisUpgrade(upgradeOption.get());
                charReceivesReward->addUpgradeComponent(upgradeOption->clone());
                stateManager->setGamePlayState(GameplayState::Play);
            });
            upgradeMenu->addButton(std::move(optionButton));
        }
        
        return upgradeMenu;
    }
    

private:
    std::unordered_map<MenuType,std::unique_ptr<Menu>> prototypes;
    std::unique_ptr<UpgradeFactory> upgradeFactory;
    TextureManager* textureManager = nullptr;
    StateManager* stateManager = nullptr;

};
