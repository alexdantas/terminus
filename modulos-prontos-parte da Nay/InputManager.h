/// @file InputManager.h Tratamento dos eventos de input (teclado)
#pragma once

#include <SDL/SDL.h>

class InputManager{
    private:
        //Existem dois vetores. Cada um tem o tamanho da quantidade de teclas do teclado
        //Para cada estado da tecla, o valor booleano se altera
        //Pressed diz se a tecla está apertada ou não
        //Change diz se houve mudança imediata entre apertar e soltar
        bool keyPressed[SDLK_LAST];
        bool keyChange[SDLK_LAST], quitGame;
        static InputManager *instance;
        SDL_Event *event;

    public:
        //Crio um evento de SDL
        //Seto fim do jogo como false
        //Seto todo vetor de teclas como false, criando-os de acordo com SDLK_LAST
        //SDLK_LAST é a quantidade de teclas do teclado atual
        InputManager();
        //Se estado da tecla foi imediatamente apertada
        bool isKeyDown(int key);
        //Se estado da tecla foi imediatamente solta
        bool isKeyUp(int key);
        //Se estado da tecla está sendo pressionada, sem mudança imediata
        bool isKeyHold(int key);
        //Retorno estado de quitGame
        bool exitGame();
        //Atualizo estados da tecla
        // Vetor   | Levantada| Pressionada
        //-------------------------------------
        // Pressed | F        | T
        // Changed | T        | T
        //_______________________________________
        // Down                  |Up                        | Hold
        // Pressionada e Alterada|Não pressionada e alterada| Pressionada e não alterada
        //
        //Toda vez que passo no loop, eu seto Changed como F
        //Se alguma ação imediata acontecer, ele se torna T
        //Pressed só se altera após alguma ação imediata
        void update();
        //Retorno somente um único objeto Input
        InputManager* getInstance();
};

