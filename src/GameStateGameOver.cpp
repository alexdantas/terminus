#include "GameStateGameOver.hpp"
#include "InputManager.hpp"
#include "Window.hpp"

GameStateGameOver::GameStateGameOver():
    font(NULL),
    text1(NULL),
    text2(NULL),
    music(NULL),
    zeus(NULL)
{ }
void GameStateGameOver::load(int stack)
{
    std::string fala;
    this->stack = stack;
    this->nextScene = 0; //First scene

    this->fadeWin  = new Fade(Fade::FADE_IN,  400, Color("white"));
    this->fadeLose = new Fade(Fade::FADE_IN, 400);

    this->win      = new Sprite("img/apterus ending 1.png");
    this->lose      = new Sprite("img/apterus ende2.png");
    this->imageTimer = new TimerCounter(2500); // 2 seconds
    //this->textTimer = new TimerCounter(5000); // 5 seconds

    this->music = new Music("ogg/gameover.ogg");

    fala = (this->stack == 0 ? "ogg/zeus_lose.ogg" : "ogg/zeus_win.ogg");
    this->zeus = new SFX(fala);

    this->gameover      = new Sprite("img/game_over.png");
    this->music->play();
    this->zeus->play();

    //this->textTimer->start();
}
int GameStateGameOver::unload()
{
    if (this->fadeWin)  delete this->fadeWin;
    if (this->fadeLose) delete this->fadeLose;
    if (this->gameover) delete this->gameover;
    if (this->win)   delete this->win;
    if (this->lose)   delete this->lose;

    if (this->imageTimer)  delete this->imageTimer;
    if (this->music) delete this->music;
    if (this->zeus) delete this->zeus;

    return 0;
}
GameState::StateCode GameStateGameOver::update(float dt)
{
    InputManager* input = InputManager::getInstance();
    input->update();

    if ((input->isKeyDown(SDLK_r)) ||
        (input->isKeyDown(SDLK_RETURN)))
        return GameState::GAME_START;

    if ((input->isKeyDown(SDLK_q)) ||
        (input->isKeyDown(SDLK_ESCAPE)))
        return GameState::QUIT;

    if (input->quitRequested())
        return GameState::QUIT;


    if(stack == 0) //Lose
        this->fadeLose->start();
    else
    {
        this->fadeWin->start();
        this->imageTimer->start();
    }

      // UPDATE

    this->win->update(dt);
    this->lose->update(dt);
    this->fadeWin->update(dt);
    this->fadeLose->update(dt);

    // if we're not doing anything it keeps TRAVANDO
    return GameState::CONTINUE;
}
void GameStateGameOver::render()
{
    if(stack == 0)
    {
        this->lose->render();
        this->fadeLose->render();
    }

    else
    {
        switch(nextScene)
        {
            case 0:
                this->win->render();
            break;

            default:
                this->lose->render();
            break;
        }

        this->fadeWin->render();

        if(this->imageTimer->isDone())
            this->nextScene = 1;
    }
            this->gameover->render();
}

