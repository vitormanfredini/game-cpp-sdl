#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderProps.h"
#include "RenderComponent.h"
#include "Alignment.h"
#include "RandomGenerator.h"
#include "GameObject/Ui/SplashScreen.h"

class SplashScreenRenderer : public RenderComponent {
private:
    SDL_Texture* logoTexture;
    SDL_Texture* backgroundTexture;
    int totalUpdates = -1;
    int fadeUpdates;
    int currentUpdates = -1;
    Uint8 alpha;

public:
    SplashScreenRenderer(
        SDL_Texture* logoTexture,
        SDL_Texture* backgroundTexture,
        int fadeUpdates
    ):
        logoTexture(logoTexture),
        backgroundTexture(backgroundTexture),
        fadeUpdates(fadeUpdates) {
        //
    }

    void update(GameObject& gameObject) override {
        currentUpdates += 1;

        if(totalUpdates == -1){
            SplashScreen* splashScreen = dynamic_cast<SplashScreen*>(&gameObject);
            if(splashScreen){
                totalUpdates = splashScreen->getTotalUpdates();
            }
        }

        if(currentUpdates < fadeUpdates){
            float fadeInPercentage = static_cast<float>(currentUpdates) / static_cast<float>(fadeUpdates);
            alpha = std::round(fadeInPercentage * 255.f);
        }else if(currentUpdates > totalUpdates - fadeUpdates){
            float fadeInPercentage = static_cast<float>(currentUpdates - totalUpdates + fadeUpdates) / static_cast<float>(fadeUpdates);
            alpha = std::max(0.0f,std::round((1.0f - fadeInPercentage) * 255.f));
        }else{
            alpha = 255;
        }
    }

    void render(GameObject& gameObject, RenderProps props) override {
        float x = gameObject.x;
        float y = gameObject.y;
        float width = gameObject.width;
        float height = gameObject.height;

        auto [offsetX, offsetY] = AlignmentUtils::computeOffsets(Alignment::Centered, width, height);

        x = x + props.leftOffset + offsetX;
        y = y + props.topOffset + offsetY;

        SDL_Rect backgroundRect = {
            static_cast<int>(0),
            static_cast<int>(0),
            static_cast<int>(std::round(1.0f * props.screenScale)),
            static_cast<int>(std::round(1.0f * props.screenScale)),
        };

        SDL_RenderCopy(props.sdl_renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_Rect logoRect = {
            static_cast<int>(std::round(x * props.screenScale)),
            static_cast<int>(std::round(y * props.screenScale)),
            static_cast<int>(std::round(width * props.screenScale)),
            static_cast<int>(std::round(height * props.screenScale)),
        };
        
        SDL_SetTextureAlphaMod(logoTexture, alpha);

        SDL_RenderCopy(props.sdl_renderer, logoTexture, NULL, &logoRect);
    }

    std::unique_ptr<RenderComponent> clone() const override {
        return std::make_unique<SplashScreenRenderer>(logoTexture, backgroundTexture, fadeUpdates);
    }
};