#pragma once

#include "Transformable.h"

enum RenderAnchor {
  GAMEWORLD,
  UI_FULLWIDTH_TOP
};

class IRenderable : public virtual Transformable {

protected:
SDL_Texture* texture = nullptr;
RenderAnchor anchor = RenderAnchor::GAMEWORLD;

public:

    void setRenderAnchor(RenderAnchor newAnchor){
        anchor = newAnchor;
    }

    RenderAnchor getRenderAnchor(){
        return anchor;
    }

    void setTexture(SDL_Texture* _texture){
        texture = _texture;
    }

    SDL_Texture* getTexture(){
        return texture;
    }

    virtual ~IRenderable() = default;

};