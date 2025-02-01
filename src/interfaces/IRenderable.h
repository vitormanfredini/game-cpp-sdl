#pragma once

#include "Transformable.h"

class IRenderable : public virtual Transformable {

protected:
SDL_Texture* texture = nullptr;

public:

    void setTexture(SDL_Texture* _texture){
        texture = _texture;
    }

    SDL_Texture* getTexture(){
        return texture;
    }

    virtual ~IRenderable() = default;

};