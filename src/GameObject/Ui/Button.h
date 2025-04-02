#pragma once

#include "GameObject/GameObject.h"
#include "MouseEventType.h"
#include <functional>
#include <iostream>
#include "ButtonState.h"

class Button: public GameObject {
private:
    std::function<void()> onClickCallback;

    bool isMouseEventInside(float mouseX, float mouseY){
        if(mouseX < getX()){
            return false;
        }
        if(mouseX > getX() + getWidth()){
            return false;
        }
        if(mouseY < getY()){
            return false;
        }
        if(mouseY > getY() + getHeight()){
            return false;
        }
        return true;
    }

    bool mouseOver = false;
    bool mouseDown = false;

public:
    Button() = default;

    ButtonState getCurrentState(){
        if(mouseDown){
            return ButtonState::Pressed;
        }
        if(mouseOver){
            return ButtonState::Hover;
        }
        return ButtonState::Idle;
    }

    void handleMouseEvent(float mouseX, float mouseY, MouseEventType eventType){
        if(eventType == MouseEventType::Motion){
            if(mouseOver != isMouseEventInside(mouseX, mouseY)){
                mouseOver = !mouseOver;
                if(!mouseOver){
                    mouseDown = false;
                }
            }
        }

        if(eventType == MouseEventType::LeftClickDown){
            if(isMouseEventInside(mouseX, mouseY)){
                mouseDown = true;
            }
        }

        if(eventType == MouseEventType::LeftClickUp){
            if(isMouseEventInside(mouseX, mouseY)){
                if(mouseDown){
                    if(onClickCallback){
                        onClickCallback();
                    }
                }
            }
        }
    }

    void setCallback(std::function<void()> onClick) {
        onClickCallback = onClick;
    }
};
