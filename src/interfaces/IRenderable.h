#pragma once

#include "GameObject.h"
#include "../RenderProps.h"

class IRenderable : public virtual GameObject {

protected:
SDL_Texture* texture = nullptr;

public:

    void setTexture(SDL_Texture* _texture){
        texture = _texture;
    }

    virtual void render(RenderProps renderProps) = 0;

    SDL_Texture* getTexture(){
        return texture;
    }

    virtual ~IRenderable() = default;

};