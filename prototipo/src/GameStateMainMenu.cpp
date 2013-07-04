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
    logo(NULL)
{ }
GameStateMainMenu::~GameStateMainMenu()
{ }
void GameStateMainMenu::load(int stack)
{
    UNUSED(stack);

    LoadingScreen loading("Loading...", "ttf/UbuntuMono.ttf");

    this->font = new Font("ttf/LithosProRegular.ttf", 42);
    this->hiliteFont = new Font("ttf/LithosProBlack.ttf", 42);

    loading.increase(12);

    this->font->setColor(Color(0, 0, 0));
    this->hiliteFont->setColor(Color(100, 100, 100));

    loading.increase(12);

    this->music = new Music("ogg/saturno.ogg");
    music->play();

    loading.increase(4);

    this->menu = new Menu(this->font, this->hiliteFont,
                          Window::width/2 - 100, Window::height/2 + 100);
    this->menu->addItem("New Game");
    this->menu->addItem("Credits");
    this->menu->addItem("Exit");
    this->menu->centralizeText();

    loading.increase(6);

    this->bg   = new Sprite("img/menu-bg.png");
    this->logo = new Sprite("img/title.png");

    loading.increase(10);

    this->clouds = new CloudContainer(30, Rectangle(0, 0, Window::width, Window::height), false);
    this->clouds->addAll();
}
int GameStateMainMenu::unload()
{
    if (this->font)
        delete this->font;

    if (this->hiliteFont)
        delete this->hiliteFont;

    if (this->music)
    {
        this->music->stop();
        delete this->music;
    }

    if (this->menu)
        delete this->menu;

    if (this->bg)
        delete this->bg;

    if (this->logo)
        delete this->logo;

    if (this->clouds)
        delete this->clouds;

    return 0;
}
int GameStateMainMenu::update(uint32_t dt)
{
    UNUSED(dt);

    InputManager* input = InputManager::getInstance();
    input->update();

    this->menu->update();

    if ((input->isKeyDown(SDLK_ESCAPE)) ||
        (input->isKeyPressed(SDLK_q))   ||
        (input->quitRequested()))
        return GameState::QUIT;

    // interacting with the menu
    if (menu->optionWasSelected())
    {
        switch (this->menu->getSelectedOption())
        {
        case 0:
            return GameState::GAME_START;
            break;

        case 1:
            break;

        case 2:
            return GameState::QUIT;
            break;

        default:
            break;
        }
    }
    this->clouds->update(dt);

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
    this->menu->render();
}

