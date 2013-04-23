#pragma once

#include <SDL/SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <time.h>
#include "geometria.h"

class SDLBase{
    private:
        static SDL_Surface *screen;
        static SDL_Surface *surface;
        static SDL_Rect *clip;
        static SDL_Rect *dst;

    public:
        static int inicializaSDL();
        SDL_Surface* getScreen();
        static SDL_Surface* loadImage(std::string arquivo);
        static void renderSurface(SDL_Surface* surface, SDL_Rect* clip = NULL, SDL_Rect* dst = NULL);
        static void atualizarTela();
        //ctor
};
