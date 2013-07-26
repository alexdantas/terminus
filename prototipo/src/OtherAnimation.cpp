#include "OtherAnimation.h"

OtherAnimation::OtherAnimation(char* filename, int n, int duracao, int newLoop) : Sprite(filename){
    n_sprites =  n;
    frameTime = duracao;
    frameSkip = 0;
    clipRect.w = (surface->w)/n_sprites;
    clipRect.h = surface->h;
    clipRect.x = clipRect.y = time = 0;
    loop = loopAux = newLoop;

}

int OtherAnimation::update(int dt){

    time += dt;

    if(frameSkip > n_sprites - 1){
        frameSkip = frameSkip % n_sprites;
        if(loop != -1)
            loopAux--;
    }

    frameSkip += time/frameTime;
    time = time % frameTime;

    clipRect.x = clipRect.w * frameSkip;

    if(clipRect.x >= surface->w)
        clipRect.x = (clipRect.x - surface->w) % surface->w;

    clip(clipRect.x, clipRect.y, clipRect.w, clipRect.h);


    return 1;
}

void OtherAnimation::setFrame(int frame){
    clipRect.x = clipRect.w * frame;
    clip(clipRect.x, clipRect.y, clipRect.w, clipRect.h);
}

void OtherAnimation::setFrameTime(int tempo){
    if(tempo >= 1)
        frameTime = tempo;
}

int OtherAnimation::getFrameTime(){
    return frameTime;
}

float OtherAnimation::getClipWidth(){
    return clipRect.w;
}

float OtherAnimation::getClipHeight(){
    return clipRect.h;
}

void OtherAnimation::restartLoop(){
    loopAux = loop;
}

bool OtherAnimation::isRunning(){
    if(loop != -1){
        return (loopAux > 0 ? true : false);
    }
    return true;
}
