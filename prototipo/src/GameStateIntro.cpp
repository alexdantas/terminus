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

    std::list<std::string>::iterator it;
    it = this->scene.begin();

    it = this->scene.insert(it, "img/intro/sequencia 1.png");
    it = this->scene.insert(it, "img/intro/sequencia 2.png");
    it = this->scene.insert(it, "img/intro/sequencia 3.png");
    it = this->scene.insert(it, "img/intro/sequencia 4.png");
    it = this->scene.insert(it, "img/intro/sequencia 5.png");
    it = this->scene.insert(it, "img/intro/sequencia 6.png");
    it = this->scene.insert(it, "img/intro/sequencia 7.png");
    it = this->scene.insert(it, "img/intro/sequencia 8.png");
    it = this->scene.insert(it, "img/intro/sequencia 9.png");
    it = this->scene.insert(it, "img/intro/sequencia 10.png");
    it = this->scene.insert(it, "img/intro/sequencia 11.png");


    this->fadeIn  = new Fade(Fade::FADE_IN,  400, Color("white"));
    this->fadeOut = new Fade(Fade::FADE_OUT, 400);

    this->image      = new Sprite("img/intro1.png");
    this->imageTimer = new TimerCounter(5000); // 2 seconds

    this->fadeInTimer = new TimerCounter(200);
    this->trasitionTimer = new TimerCounter(500);
    this->fadeInTimer->startCounting();
    this->music = new Music("ogg/op.ogg");
    this->music->play();
}
int GameStateIntro::unload()
{
    if (this->fadeIn)  delete this->fadeIn;
    if (this->fadeOut) delete this->fadeOut;
    if (this->image)   delete this->image;
    if (this->music)    delete this->music;

    if (this->fadeInTimer) delete this->fadeInTimer;
    if (this->imageTimer)  delete this->imageTimer;
    if (this->trasitionTimer)  delete this->trasitionTimer;
    return 0;
}
GameState::StateCode GameStateIntro::update(float dt)
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
    if (this->imageTimer->isDone() || this->trasitionTimer->isDone())
    {
        this->fadeIn->start();
        if(!this->scene.empty())
        {
            this->image->setFilename(this->scene.back());
            this->image->load();
            this->scene.pop_back();
            this->trasitionTimer->start();
        }
        else
        {
            this->fadeIn->stop();
            this->fadeOut->start();
        }

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

