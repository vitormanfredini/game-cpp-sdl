#pragma once

#include "GameObject/GameObject.h"
#include "MouseEventType.h"
#include "Button.h"

class Menu: public GameObject {

private:
    std::vector<std::unique_ptr<Button>> buttons;

public:
    void handleMouseEvent(float mouseX, float mouseY, MouseEventType eventType){
        for(std::unique_ptr<Button>& button : buttons){
            button->handleMouseEvent(mouseX, mouseY, eventType);
        }
    }

    void addButton(std::unique_ptr<Button> button) {
        buttons.push_back(std::move(button));
    }

    void render(RenderProps renderProps) override {
        GameObject::render(renderProps);
        
        for (auto& button : buttons) {
            button->render(renderProps);
        }
    }

};
