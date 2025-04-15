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

    std::unique_ptr<Menu> clone() {
        std::unique_ptr<Menu> copy = std::make_unique<Menu>();
        copy->x = x;
        copy->y = y;
        copy->width = width;
        copy->height = height;

        for(std::unique_ptr<RenderComponent>& renderComponent : renderComponents){
            std::unique_ptr<RenderComponent> clonedRenderComponent = renderComponent->clone();
            copy->addRenderComponent(std::move(clonedRenderComponent));
        }

        for(std::unique_ptr<Button>& button : buttons){
            std::unique_ptr<Button> clonedButton = button->clone();
            copy->addButton(std::move(clonedButton));
        }

        return copy;
    };

};
