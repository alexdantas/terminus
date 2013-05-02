#include "../include/InputManager.h"

InputManager *InputManager::instance = NULL;

void falseVector(bool vec[], int arg){
    for(int i = 0 ; i < arg; i++)
        vec[i] = false;
}

InputManager::InputManager(){
    quitGame = false;
    event = new SDL_Event;

    falseVector(keyChange, SDLK_LAST);
    falseVector(keyPressed, SDLK_LAST);

}

InputManager* InputManager::getInstance(){
    if(!instance)
        instance = new InputManager();

    return instance;
}


bool InputManager::isKeyDown(int key){
    return (keyChange[key] && keyPressed[key]);
}
bool InputManager::isKeyUp(int key){
    return (keyChange[key] && !keyPressed[key]);
}
bool InputManager::isKeyHold(int key){
    return (!keyChange[key] && keyPressed[key]);
}

bool InputManager::exitGame(){
    return quitGame;
}

void InputManager::update(){
    falseVector(keyChange, SDLK_LAST);

    if(SDL_PollEvent(event)){
        switch(event->type){
        case SDL_KEYDOWN:
            keyChange[event->key.keysym.sym] = true;
            keyPressed[event->key.keysym.sym] = true;
        break;
        case SDL_KEYUP:
            keyChange[event->key.keysym.sym] = true;
            keyPressed[event->key.keysym.sym] = false;
        break;
        case SDL_QUIT:
            quitGame = true;
        break;

        }
    }
       if(isKeyDown(SDLK_ESCAPE)){
        quitGame = true;
    }
}
