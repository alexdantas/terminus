#include "../include/SDLBase.h"

SDL_Surface *SDLBase::screen = NULL;
SDL_Surface *SDLBase::surface = NULL;

int SDLBase::inicializaSDL(){
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER);

  //  freopen("CON", "w", stdout); // redirects stdout
  //  freopen("CON", "w", stderr); // redirects stderr

    screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Prototipo - IDJogo", "Prototipo - IDJogo");

    return 1;
}

SDL_Surface* SDLBase::getScreen(){
    return screen;
}

SDL_Surface* SDLBase::loadImage(std::string arquivo){
    SDL_Surface *surfaceaux = 0;

    surface = IMG_Load(arquivo.c_str());

    surfaceaux = ((surface->format->Amask != 0) ? SDL_DisplayFormatAlpha(surface) : SDL_DisplayFormat(surface));
    SDL_FreeSurface(surface);

    return surfaceaux;
}

void SDLBase::renderSurface(SDL_Surface *surface, SDL_Rect *clip, SDL_Rect *dst){
    SDL_BlitSurface(surface, clip, screen, dst);
}

void SDLBase::atualizarTela(){
    SDL_Flip(screen);
}

