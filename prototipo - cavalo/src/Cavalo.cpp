#include "../include/Cavalo.h"

Cavalo::Cavalo(Sprite *figura, float HP, float posicaoX, float posicaoY){
    sprite = figura;
    vida = HP;
    x = posicaoX;
    y = posicaoY;
}

void Cavalo::render(Point camera){
    sprite->render(x - camera.x, y - camera.y);
}

void Cavalo::setX(float posicaoX){
    x = posicaoX;
}

void Cavalo::setY(float posicaoY){
    y = posicaoY;
}

void Cavalo::andeX(float posicaoX){
    x += posicaoX;
}

void Cavalo::andeY(float posicaoY){
    y += posicaoY;
}

float Cavalo::getVida(){
    return vida;
}

void Cavalo::levaDano(float dano){
    vida -= dano;
}

float Cavalo::getX(){
    return x;
}

float Cavalo::getY(){
    return y;
}

Sprite* Cavalo::getSprite(){
    return sprite;
}
