#pragma once

#include "SDLbase.h"

class Sprite{
    private:
        SDL_Surface *surface;
        SDL_Rect clipRect;

    public:
        Sprite(std::string arquivo);
        ~Sprite();
        void load(std::string arquivo);
        void clip(int x, int y, int w, int h);
        SDL_Rect getClip();
        int getWidth();
        int getHeight();
        void render(int x, int y);
};
