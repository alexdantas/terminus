#include "GameStateIntro.hpp"
#include "InputManager.hpp"
#include "Window.hpp"

GameStateIntro::GameStateIntro():
    fadeIn(NULL),
    fadeOut(NULL),
    image(NULL),
    imageTimer(NULL),
    fadeInTimer(NULL)
{ }
void GameStateIntro::load(int stack)
{
    UNUSED(stack);

    this->fadeIn  = new Fade(Fade::FADE_IN,  0.3, Color("white"));
    this->fadeOut = new Fade(Fade::FADE_OUT, 0.4);

    this->image      = new Sprite("img/intro1.png");
    this->imageTimer = new TimerCounter(2000); // 2 seconds

    this->fadeInTimer = new TimerCounter(500);
    this->fadeInTimer->startCounting();
}
int GameStateIntro::unload()
{
    if (this->fadeIn)  delete this->fadeIn;
    if (this->fadeOut) delete this->fadeOut;
    if (this->image)   delete this->image;

    if (this->fadeInTimer) delete this->fadeInTimer;
    if (this->imageTimer)  delete this->imageTimer;
    return 0;
}
GameState::StateCode GameStateIntro::update(uint32_t dt)
{
    // TIMERS
    // How long the screen is black before the fade in.
    if (this->fadeInTimer->isDone())
    {
        this->fadeIn->start();
        this->imageTimer->startCounting();
    }

    // Fade out starts when the image has spent enough time
    // on the screen.
    if (this->imageTimer->isDone())
    {
        this->fadeIn->stop(); // safety check - if the timer's
                              // not enough, this will make the
                              // screen flash.
        this->fadeOut->start();
    }

    // We only quit to main menu after the fade out completes.
    if (this->fadeOut->isDone())
        return GameState::MAIN_MENU;

    // INPUT
    InputManager* input = InputManager::getInstance();
    input->update();

    // Player can skip to the main menu anytime by pressing
    // ENTER or SPACE BAR.
    if ((input->isKeyDown(SDLK_RETURN)) ||
        (input->isKeyDown(SDLK_SPACE)))
        return GameState::MAIN_MENU;

    // Player can quit the game anytime by pressing Q or ESC.
    if ((input->isKeyDown(SDLK_q)) ||
        (input->isKeyDown(SDLK_ESCAPE)))
        return GameState::QUIT;

    // Player can also quit by pressing ALT+F4 or clicking the
    // X on the window.
    if (input->quitRequested())
        return GameState::QUIT;

    // UPDATE
    this->image->update(dt);
    this->fadeIn->update(dt);
    this->fadeOut->update(dt);

    return GameState::CONTINUE;
}
void GameStateIntro::render()
{
    this->image->render();
    this->fadeIn->render();
    this->fadeOut->render();
}

