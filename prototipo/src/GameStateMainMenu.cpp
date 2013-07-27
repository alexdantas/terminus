#include "GameStateMainMenu.hpp"
#include "InputManager.hpp"
#include "SDL.hpp"
#include "LoadingScreen.hpp"
#include "Window.hpp"

GameStateMainMenu::GameStateMainMenu():
    font(NULL),
    music(NULL),
    menu(NULL),
    bg(NULL),
    logo(NULL),
    detail(NULL)
{ }
GameStateMainMenu::~GameStateMainMenu()
{ }
void GameStateMainMenu::load(int stack)
{
    UNUSED(stack);

    LoadingScreen loading("Loading...");
    loading.setBg("img/loading.png");
    loading.increase(0);
    loading.setSubtitle("loading loading screen...");

    this->font = new Font("ttf/LithosProRegular.ttf", 42);
    this->hiliteFont = new Font("ttf/LithosProBlack.ttf", 42);

    loading.increase(10);
    loading.setSubtitle("loading fonts...");

    this->font->setColor(Color(0, 0, 0));
    this->hiliteFont->setColor(Color(100, 100, 100));

    loading.increase(12);
    loading.setSubtitle("loading colors...");

    this->bg   = new Sprite("img/menu-bg.png");
    this->logo = new Sprite("img/title.png");
    this->detail = new Sprite("img/pilar.png");

    loading.increase(12);
    loading.setSubtitle("loading sprites...");

    this->music = new Music("ogg/saturno.ogg");
//    music->play();

    loading.increase(4);
    loading.setSubtitle("loading music...");

    this->menu = new Menu(this->font, this->hiliteFont,
                          Window::width/2 - 100,
                          Window::height/2 + 100);

    this->menu->addItem("New Game");
    this->menu->addItem("Instructions");
    this->menu->addItem("Exit");
    this->menu->centralizeText();

    loading.increase(6);
    loading.setSubtitle("loading menu...");

    this->clouds = new CloudContainer(30, Rectangle(0, 0, Window::width, Window::height), false);
    this->clouds->addAll();

    this->fade = new Fade(Fade::FADE_IN, 200);
    this->fade->start();

    this->fadeOut = new Fade(Fade::FADE_OUT, 500);

    this->bgmusic = new Music("ogg/intro.ogg");
    this->bgmusic->play();

    loading.increase(10);
    loading.setSubtitle("loading all the rest...");
}
int GameStateMainMenu::unload()
{
// Time to delete!
// This macro deletes a thing only if it's non-NULL,
// making sure it won't double-delete things.
#define safe_delete(x) \
{                      \
    if (x)             \
    {                  \
        delete (x);    \
        x = NULL;      \
    }                  \
}
    safe_delete(this->font);
    safe_delete(this->hiliteFont);

    if (this->music)
    {
        this->music->stop();
        safe_delete(this->music);
    }

    safe_delete(this->menu);
    safe_delete(this->bg);
    safe_delete(this->logo);
    safe_delete(this->clouds);
    safe_delete(this->detail);
    safe_delete(this->fade);
    safe_delete(this->fadeOut);

    return 0;
}
GameState::StateCode GameStateMainMenu::update(float dt)
{
    UNUSED(dt);

    InputManager* input = InputManager::getInstance();
    input->update();

    this->menu->update();

    if ((input->isKeyDown(SDLK_ESCAPE)) ||
        (input->isKeyPressed(SDLK_q))   ||
        (input->quitRequested()))
        this->fadeOut->start();

    // interacting with the menu
    if (menu->optionWasSelected())
    {
        switch (this->menu->getSelectedOption())
        {
        case 0:
            return GameState::GAME_START;
            break;

        case 1:
            return GameState::GAME_INSTRUCTION;
            break;

        case 2:
//            return GameState::QUIT;
            this->fadeOut->start();
            break;

        default:
            break;
        }
    }
    this->clouds->update(dt);
    this->fade->update(dt);
    this->fadeOut->update(dt);

    if (this->fadeOut->isDone())
        return GameState::QUIT;

    return GameState::CONTINUE;
}
void GameStateMainMenu::render()
{
    // this->font->print(30, 30, "Main Menu");

    // this->font->print(100, 300, "IT WORKS!");

    // this->font->print(400, 400,
    //                   "Aperte 'up' pra iniciar,"
    //                   "'q' para sair.");
    this->bg->render(0, 0);
    this->clouds->render();
    this->logo->render(0, 0);
    this->detail->render(0, 0);
    this->menu->render();

    this->fade->render(); // Always last
    this->fadeOut->render(); // Always last
}

