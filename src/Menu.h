#pragma once

#include "GameObject/GameObject.h"
#include "MouseEventType.h"

class Menu: public GameObject {

public:
    void handleMouseEvent(float mouseX, float mouseY, MouseEventType eventType){
        if(eventType == MouseEventType::LeftClickDown){
            std::cout << "MouseEventType::LeftClickDown" << std::endl;
        }
        if(eventType == MouseEventType::LeftClickUp){
            std::cout << "MouseEventType::LeftClickUp" << std::endl;
        }
        if(eventType == MouseEventType::RightClickDown){
            std::cout << "MouseEventType::RightClickDown" << std::endl;
        }
        if(eventType == MouseEventType::RightClickUp){
            std::cout << "MouseEventType::RightClickUp" << std::endl;
        }
        if(eventType == MouseEventType::Motion){
            std::cout << "MouseEventType::Motion" << std::endl;
        }
        std::cout << mouseX << " - " << mouseY << std::endl;
    }
};
