#include "GameStateIntro.hpp"
#include "InputManager.hpp"
#include "Window.hpp"

GameStateIntro::GameStateIntro():
    fadeIn(NULL),
    fadeOut(NULL),
    image(NULL),
    timer(NULL)
{ }
void GameStateIntro::load(int stack)
{
    UNUSED(stack);

    this->fadeIn  = new Fade(Fade::FADE_IN,  0.2);
    this->fadeOut = new Fade(Fade::FADE_OUT, 0.2);

    this->image = new Sprite("img/menu-bg.png");

    this->timer = new Timer();

    this->fadeIn->start();
    this->timer->start();
}
int GameStateIntro::unload()
{
    if (this->fadeIn) delete this->fadeIn;
    if (this->fadeOut) delete this->fadeOut;
    if (this->image) delete this->image;

    return 0;
}
int GameStateIntro::update(uint32_t dt)
{
    // INPUT
    InputManager* input = InputManager::getInstance();
    input->update();

    if (input->isKeyDown(SDLK_RETURN))
        return GameState::MAIN_MENU;

    if ((input->isKeyDown(SDLK_q)) ||
        (input->isKeyDown(SDLK_ESCAPE)))
        return GameState::QUIT;

    if (input->quitRequested())
        return GameState::QUIT;

    // TIMER
    this->timer->pause();

    // Let's exit if it took too long
    if (this->timer->delta() > 2000)
    {
        this->fadeIn->stop(); // safety check
        this->fadeOut->start();
    }
    else
        this->timer->unpause();

    // UPDATE
    this->image->update(dt);
    this->fadeIn->update(dt);
    this->fadeOut->update(dt);

    // We only quit after the fade out.
    if (this->fadeOut->isDone())
        return GameState::MAIN_MENU;

    // if we're not doing anything it keeps TRAVANDO
    return GameState::CONTINUE;
}
void GameStateIntro::render()
{
    this->image->render();
    this->fadeIn->render();
    this->fadeOut->render();
}

