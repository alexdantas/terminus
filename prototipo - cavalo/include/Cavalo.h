#pragma once

#include "SDLbase.h"
#include "Sprite.h"

class Cavalo{
    private:
        Sprite *sprite;
        float x, y, vida;

    public:
        Cavalo(Sprite *sprite, float vida, float x, float y);
        void render(Point);
        Sprite* getSprite();
        float getVida();
        void levaDano(float dano);
        float getX();
        float getY();
        void setX(float posicaoX);
        void setY(float posicaoY);
        void andeX(float posicaoX);
        void andeY(float posicaoY);
};

