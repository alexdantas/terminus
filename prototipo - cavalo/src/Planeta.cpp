#include "../include/Planeta.h"

Planeta::Planeta(Sprite *figura, float vida, float posicaoX, float posicaoY){
    sprite = figura;
    hitpoints = vida;
    x = posicaoX;
    y = posicaoY;
}

void Planeta::render(int posicaoX, int posicaoY){
    sprite->render(x - posicaoX, y - posicaoY);
}

float Planeta::getHitpoints(){
    return hitpoints;
}

void Planeta::levaDano(float dano){
    hitpoints -= dano;
}

float Planeta::getX(){
    return x;
}

float Planeta::getY(){
    return y;
}

Sprite* Planeta::getSprite(){
    return sprite;
}
