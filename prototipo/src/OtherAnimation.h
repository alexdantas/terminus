#pragma once

#include "Sprite.hpp"
#include "GameObject.hpp"

class OtherAnimation : public Sprite{
    private:
        int n_sprites, time, frameSkip, frameTime, loop, loopAux;
        SDL_Rect clipRect;

    public:
        OtherAnimation(char* filename, int n_sprites, int frameTime, int loop = -1);
        int update(int dt);
        void setFrame(int frame);
        void setFrameTime(int time);
        void restartLoop();
        int getFrameTime();
        float getClipWidth();
        float getClipHeight();
        bool isRunning();
};
