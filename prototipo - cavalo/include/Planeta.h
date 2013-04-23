#pragma once

#include "SDLbase.h"
#include "Sprite.h"

class Planeta{
    private:
        Sprite *sprite;
        float x, y, hitpoints;

    public:
        Planeta(Sprite *sprite, float hitpoints, float x, float y);
        void render(int posicaoX, int posicaoY);
        float getHitpoints();
        void levaDano(float dano);
        float getX();
        float getY();
        Sprite* getSprite();

};
