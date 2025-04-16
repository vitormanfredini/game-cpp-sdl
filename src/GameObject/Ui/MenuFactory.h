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

        std::unique_ptr<Button> startButton = std::make_unique<Button>();
        startButton->setPosition(0.33f, 0.3f);
        startButton->setSize(0.33f, 0.166f);
        startButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->loadTexture("images/button_start.png")
        ));
        startButton->setCallback([stateManager]() {
            std::cout << "MainMenu: startButton" << std::endl;
            stateManager->setMainState(MainState::Gameplay);
        });
        prototypes[MenuType::MainMenu]->addButton(std::move(startButton));

        std::unique_ptr<Button> exitButton = std::make_unique<Button>();
        exitButton->setPosition(0.33f, 0.53f);
        exitButton->setSize(0.33f, 0.166f);
        exitButton->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->loadTexture("images/button_exit.png")
        ));
        exitButton->setCallback([stateManager]() {
            std::cout << "MainMenu: exitButton" << std::endl;
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

    std::unique_ptr<Menu> createUpgradeMenu(Character* character) {
        std::unique_ptr<Menu> upgradeMenu = prototypes[MenuType::UpgradeMenu]->clone();

        std::unique_ptr<Button> option1Button = std::make_unique<Button>();
        option1Button->setPosition(0.33f, 0.2f);
        option1Button->setSize(0.33f, 0.166f);
        option1Button->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->loadTexture("images/button_upgrademenu_option.png")
        ));
        option1Button->setCallback([this, character]() {
            character->addUpgradeComponent(std::make_unique<UpgradeComponent>(StatType::MaxHealth, 1, 0.05));
            stateManager->setGamePlayState(GameplayState::Play);
        });
        upgradeMenu->addButton(std::move(option1Button));

        std::unique_ptr<Button> option2Button = std::make_unique<Button>();
        option2Button->setPosition(0.33f, 0.4f);
        option2Button->setSize(0.33f, 0.166f);
        option2Button->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->loadTexture("images/button_upgrademenu_option.png")
        ));
        option2Button->setCallback([this, character]() {
            character->addUpgradeComponent(std::make_unique<UpgradeComponent>(StatType::MaxHealth, 1, 2.0));
            stateManager->setGamePlayState(GameplayState::Play);
        });
        upgradeMenu->addButton(std::move(option2Button));

        std::unique_ptr<Button> option3Button = std::make_unique<Button>();
        option3Button->setPosition(0.33f, 0.6f);
        option3Button->setSize(0.33f, 0.166f);
        option3Button->addRenderComponent(std::make_unique<ButtonRenderer>(
            textureManager->loadTexture("images/button_upgrademenu_option.png")
        ));
        option3Button->setCallback([this, character]() {
            character->addUpgradeComponent(std::make_unique<UpgradeComponent>(StatType::MaxHealth, 1, 2.0));
            stateManager->setGamePlayState(GameplayState::Play);
        });
        upgradeMenu->addButton(std::move(option3Button));
        return upgradeMenu;
    }
    

private:
    std::unordered_map<MenuType,std::unique_ptr<Menu>> prototypes;
    std::unique_ptr<UpgradeFactory> upgradeFactory;
    TextureManager* textureManager = nullptr;
    StateManager* stateManager = nullptr;

};
