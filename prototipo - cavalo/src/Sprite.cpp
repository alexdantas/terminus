#include "../include/Sprite.h"

Sprite::Sprite(std::string arquivo){
    surface = NULL;

    load(arquivo);
}
Sprite::~Sprite(){
    if(surface != NULL)
        SDL_FreeSurface(surface);
}

void Sprite::load(std::string arquivo){
    if(surface != NULL)
        SDL_FreeSurface(surface);

    surface = SDLBase::loadImage(arquivo);

    clipRect.x = clipRect.y = 0;
    clipRect.w = surface->w;
    clipRect.h = surface->h;
}

void Sprite::clip(int x, int y, int w, int h){
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

SDL_Rect Sprite::getClip(){
    return clipRect;
}

void Sprite::render(int x, int y){
    SDL_Rect dst;

    dst.x = x;
    dst.y = y;
    dst.w = dst.h = 0;

    SDLBase::renderSurface(surface, &clipRect, &dst);
}

int Sprite::getWidth(){
    return surface->w;
}


int Sprite::getHeight(){
    return surface->h;
}
