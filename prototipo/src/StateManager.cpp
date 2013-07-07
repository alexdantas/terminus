#include <cstdlib> // srand()
#include <ctime>   // time()
#include "StateManager.hpp"
#include "SDL.hpp"
#include "Log.hpp"
#include "Config.hpp"
#include "GameStateMainMenu.hpp"
#include "GameStateGame.hpp"
#include "GameStateGameOver.hpp"
#include "GameStateIntro.hpp"
#include "Window.hpp"
#include "Graphics.hpp"

StateManager::StateManager(int width, int height)
{
    Config::load("config.ini");

    if (Config::debugMode)
        Log::debugMode(true);

    SDL::init(30); // framerate

    Window::init(width, height, "PROTOTYPE", "prototype");
    Graphics::init(Window::screen);

    // For even further debugging purposes
    Log::verboseMode(true);

    // Starting the game!
    if (Config::skipMenu)
        this->currentState = new GameStateGame();
    else
        this->currentState = new GameStateIntro();

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

        this->currentState = NULL; // you never know
    }
}
void StateManager::run()
{
    bool letsQuit = false;

    bool firstFrame = true;

    while (!letsQuit)
    {
        // The delta time from the last frame
//        uint32_t delta = SDL::getDelta();

        // How many seconds have passed between last frame and this one?
        float delta = SDL::getDeltaSeconds();

        // Little hack to avoid things jumping around on the
        // first frame.
        if (firstFrame)
        {
            delta = 1000.0/30.0;
            firstFrame = false;
        }

        // Just a reminder that every state handles input
        // individually, so no need to do that here.

        // Updating the whole state.
        // This value is returned from it tell us if
        // we need to switch from the current state.
        GameState::StateCode whatToDoNow = this->currentState->update(delta);

        switch (whatToDoNow)
        {
        case GameState::CONTINUE:
            // Just continue on the current state.
            break;

        case GameState::QUIT:
            letsQuit = true;
            break;

        case GameState::GAME_START: // yay, the actual game!
            this->sharedInfo = this->currentState->unload();
            delete (this->currentState);

            this->currentState = new GameStateGame();
            this->currentState->load(this->sharedInfo);
            break;

        case GameState::GAME_OVER: // baww, we lost!
            this->sharedInfo = this->currentState->unload();
            delete (this->currentState);

            this->currentState = new GameStateGameOver();
            this->currentState->load(this->sharedInfo);
            break;

        case GameState::MAIN_MENU: // nothing exciting here
            this->sharedInfo = this->currentState->unload();
            delete (this->currentState);

            this->currentState = new GameStateMainMenu();
            this->currentState->load(this->sharedInfo);
            break;

        default:
            break;
        }

        // Printing everything on the screen.
        Window::clear();
        this->currentState->render();
        Window::refresh();

        // Let's wait a bit if the framerate is too low.
        SDL::framerateWait();
    }
}

