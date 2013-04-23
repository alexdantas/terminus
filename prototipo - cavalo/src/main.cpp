#include "../include/GameManager.h"


int main ( int argc, char** argv ){

     if(SDLBase::inicializaSDL()){
        GameManager controleJogo;
        controleJogo.run();
     }

    return 0;
}
