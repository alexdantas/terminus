#include <cstdlib> // srand()
#include <ctime>   // time()
#include "StateManager.hpp"
#include "SDL.hpp"
#include "Log.hpp"
#include "Config.hpp"
#include "GameStateMainMenu.hpp"
#include "GameStateGame.hpp"
#include "GameStateGameOver.hpp"
#include "Window.hpp"
#include "Graphics.hpp"

StateManager::StateManager(int width, int height)
{
    Config::load("config.ini");
    if (Config::debugMode)
        Log::debugMode(true);

    SDL::init(30);

    Window::init(width, height, "PROTOTYPE", "prototype");
    Graphics::init(Window::screen);

    Log::verboseMode(true);

    // Here we start the game!
    // The config file tells if we're going to skip themain menu
    if (Config::skipMenu)
        this->currentState = new GameStateGame();
    else
        this->currentState = new GameStateMainMenu();

    this->currentState->load();

    this->sharedInfo = 0;

    srand(time(NULL));
}
StateManager::~StateManager()
{
    SDL::exit();

    if (this->currentState)
    {
        this->currentState->unload();
        delete this->currentState;
    }
}
void StateManager::run()
{
    bool letsQuit = false;

    while (!letsQuit)
    {
        // The delta time from the last frame
        uint32_t delta = SDL::getDelta();

        // Normally i'd refresh the input manager right here, but
        // each state must do it individually.

        int whatToDoNow = this->currentState->update(delta);

        switch (whatToDoNow)
        {
        case GameState::CONTINUE:
            break;

        case GameState::QUIT:
            letsQuit = true;
            break;

        case GameState::GAME_START:
            this->sharedInfo = this->currentState->unload();
            delete (this->currentState);

            this->currentState = new GameStateGame();
            this->currentState->load(this->sharedInfo);
            break;

        case GameState::GAME_OVER:
            this->sharedInfo = this->currentState->unload();
            delete (this->currentState);

            this->currentState = new GameStateGameOver();
            this->currentState->load(this->sharedInfo);
            break;

        case GameState::MAIN_MENU:
            this->sharedInfo = this->currentState->unload();
            delete (this->currentState);

            this->currentState = new GameStateMainMenu();
            this->currentState->load(this->sharedInfo);
            break;

        default:
            break;
        }

        // Printing everything on the screen
        Window::clear();
        this->currentState->render();
        Window::refresh();

        // Let's wait a bit if the framerate is too low.
        SDL::framerateWait();
    }
}

