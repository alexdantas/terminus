#include "GameStateGame.hpp"
#include "StateManager.hpp"
#include "Config.hpp"
#include "Log.hpp"
#include "Window.hpp"
#include "LoadingScreen.hpp"

bool GameStateGame::showBoundingBoxes = false;

// All possible commands the user might type into the console
enum GameStateGameCommands
{
    COMMAND_KILL, COMMAND_QUIT, COMMAND_ADD_PLANET
};

GameStateGame::GameStateGame():
    bg(NULL),
    planetSprite(NULL),
    earthSprite(NULL),
    moonSprite(NULL),
    ufoSprite(NULL),
    shipAnim(NULL),
    explosionAnim(NULL),
    earth(NULL),
    moon(NULL),
    will_quit(false),
    game_over(false),
    camera(NULL),
    ufo(NULL),
    bgmusic(NULL),
    ship(NULL),
    explosion(NULL),
    lifeBar(NULL),
    lifeBarFont(NULL),
    lifeBarText(NULL),
    isPaused(false),
    font(NULL),
    pausedTitle(NULL),
    console(NULL)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
    UNUSED(stack);

    LoadingScreen loading("loading...", "ttf/Terminus.ttf");

    loading.increase(2);

    this->bg = new Sprite("img/cenario.png");

    loading.increase(30);

    this->planetSprite = new Sprite("img/PlanetaVermelho.png");

    loading.increase(3);

    loading.increase(4);

    this->earthSprite = new Sprite("img/PlanetaTerra.png");
    this->earthSprite->rotoZoom(0, 2.0, 2.0);
    this->earth = new Earth(this->earthSprite, 20,
                            SDL::randomNumberBetween(-1000, 1000),
                            SDL::randomNumberBetween(-1000, 1000),
                            this->earthSprite->getWidth(),
                            this->earthSprite->getHeight());

    loading.increase(8);

    this->moonSprite = new Sprite("img/Lua.png");
    this->moon       = new Moon(this->moonSprite, 1, this->earth,
                                this->moonSprite->getWidth(),
                                this->moonSprite->getHeight());

    loading.increase(8);

    this->camera = new Camera(0, 0, Window::width,
                              Window::height,
                              Config::cameraScrollSpeed);
    this->camera->lockXAxis();

    loading.increase(3);

    this->ufoSprite = new Sprite("img/ufo.png");
    this->ufo = new FollowerObject(this->ufoSprite, 30, 30,
                                   this->ufoSprite->getWidth(),
                                   this->ufoSprite->getHeight(), 20);

    loading.increase(8);

    int playerX = 100;
    int playerY = this->bg->getHeight() - 100;

    this->shipAnim = new Animation("img/NaveSheet.png", 4, 18);
    this->ship = new AccellObject(this->shipAnim,
                                  playerX, playerY,
                                  94, 100, 20);

    loading.increase(10);

    this->explosionAnim = new Animation("img/explosion.png", 22, 30);
    this->explosion = new Explosion(this->explosionAnim);

    loading.increase(8);

    this->lifeBar = new ProgressBar(200, 20, this->ship->getHitpoints(), this->ship->getHitpoints());
    this->lifeBar->setForegroundColor(Color(255, 0, 255));
    this->lifeBar->setBackgroundColor(Color(100, 0, 100));

    loading.increase(4);

    this->lifeBarFont = new Font("ttf/UbuntuMono.ttf", 16);
    this->lifeBarText = new Text(this->lifeBarFont);
    this->lifeBarText->setText("Energy");
    this->lifeBarText->setPosition(10, 10);

    loading.increase(4);

    this->font = new Font("ttf/UbuntuMono.ttf", 42);

    loading.increase(5);

    this->pausedTitle = new Text(this->font);
    this->pausedTitle->setText("Paused");
    this->pausedTitle->setPosition((Window::width/2) - 50,
                                   (Window::height/2) - 100);

    loading.increase(5);

    this->hiliteFont = new Font("ttf/UbuntuMono.ttf", 42);
    this->hiliteFont->setColor(Color(100, 100, 100));

    loading.increase(5);

    this->pausedMenu = new Menu(this->font, this->hiliteFont,
                                Window::width/2  - 60,
                                Window::height/2 + 100);

    this->pausedMenu->addItem("Continue");
    this->pausedMenu->addItem("Exit");
    this->pausedMenu->centralizeText();

    loading.increase(13);

    this->bgmusic = new Music("ogg/escaping.ogg");
    this->bgmusic->play();

    loading.increase(10);

    this->cenario = new Background("img/cenario.png");

    loading.increase(10);

    this->consoleFont = new Font("ttf/UbuntuMono.ttf", 18);

    this->console = new DropDownConsole(this->consoleFont, 0, 0, Window::width, Window::height/3);
    this->console->setPromptForegroundColor(Color(20, 230, 20));
    this->console->setForegroundColor(Color(20, 180, 20));
    this->console->setBackgroundColor(Color(20, 20, 20, 210));
    this->console->setScrollSpeed(1.78);
    this->console->refresh();
    this->console->print("* Game console loaded *");

    this->console->addCommand("kill", COMMAND_KILL);
    this->console->addCommand("quit", COMMAND_QUIT);
    this->console->addCommand("addplanet", COMMAND_ADD_PLANET);
}
int GameStateGame::unload()
{
    // Before deleting everything, we must see if we
    // just won.
    // If we did, then this->ufo will be NULL
    bool we_won = false;

    if (!(this->ufo))
        we_won = true;

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

    int size = this->planetArray.size();

    for (int i = 0; i < size; i++)
        safe_delete(this->planetArray[i]);

    safe_delete(this->planetSprite);

    safe_delete(this->bg);
    safe_delete(this->bgmusic);
    safe_delete(this->camera);

    safe_delete(this->earth);
    safe_delete(this->earthSprite);

    safe_delete(this->moon);
    safe_delete(this->moonSprite);

    safe_delete(this->ship);
    safe_delete(this->shipAnim);

// TODO there's a segmentation fault when I delete explosion
//      why is that?
//    safe_delete(this->explosion);

    safe_delete(this->explosionAnim);

    safe_delete(this->font);
    safe_delete(this->pausedTitle);
    safe_delete(this->hiliteFont);
    safe_delete(this->pausedMenu);

    safe_delete(this->lifeBar);
    safe_delete(this->lifeBarFont);
    safe_delete(this->lifeBarText);

    safe_delete(this->cenario);

    if (we_won)
        return 1;
    else
        return 0;
}
int GameStateGame::update(uint32_t dt)
{
    if (this->will_quit)
        return GameState::QUIT;

    // Player/UFO has died.
    // Will only go to the GAME OVER screen when the
    // explosion ends.
    if (this->game_over)
        if (!(this->explosion->isExploding()))
            return GameState::GAME_OVER;

    this->processEvents();

    this->console->update(dt);

    int command = this->console->hasCommand();
    if (command)
    {
        switch (this->console->getCommand())
        {
        case COMMAND_KILL:
            this->explosion->explodeAt(this->ship->getX(),
                                       this->ship->getY());
            break;
        case COMMAND_QUIT:
            this->console->print("Quitting...");
            this->will_quit = true;
            break;

        case COMMAND_ADD_PLANET:
            this->addPlanet();
            break;

        default:
            this->console->print("Unknown Command!");
            break;
        }
    }
    if (this->isPaused)
    {
        this->pausedMenu->update();
        return GameState::CONTINUE;
    }

    // Things that are up there updates independently if the
    // game's paused.
    // From now on, they won't.

    int size = this->planetArray.size();
    for (int i = 0; i < size; i++)
        this->planetArray[i]->update(dt);

    this->earth->update(dt);
    this->moon->update(dt);

    if (this->ufo)
        this->ufo->update(dt);

    if (this->ship)
        this->ship->update(dt);

    this->camera->update(dt);

    float levelHeight  = this->bg->getHeight();

    float cameraLowestPoint = this->camera->getY() + this->camera->getHeight();
    float cameraScrollPoint = cameraLowestPoint    - this->camera->getHeight()/3;

    this->camera->centerOn(this->ship->getCenterX(),
                           this->ship->getCenterY());

//    if (cameraLowestPoint > levelHeight)
//        this->camera->setY(levelHeight - this->camera->getHeight());

//     // Relative-stuff
//     float cameraOffset  = this->camera->getY();
//     float scrollPoint   = 200; // relative to the camera
//     float playerCenterY = (this->ship->getY() + this->ship->getHeight()/2) - cameraOffset;

//     if (playerCenterY < scrollPoint)
//         this->camera->scroll(Camera::UP, (playerCenterY - cameraOffset));
// //        this->camera->setY(playerCenterY);

//     // this->camera->centerOn(this->ship->getX() + (this->ship->getWidth()/2),
//     //                        this->ship->getY() + (this->ship->getHeight()/2));

//     // Global stuff
//     float cameraLowestPoint = cameraOffset + this->camera->getY();
//     float levelLowestPoint  = this->bg->getHeight();

//     if (cameraLowestPoint > levelLowestPoint)
//         this->camera->setY(levelLowestPoint - cameraOffset);

    if (this->ship->getY() >= cameraLowestPoint)
    {
        // this is where the player dies
    }

    this->checkCollision();
    this->explosion->update(dt);

    return GameState::CONTINUE;
}
void GameStateGame::render()
{
    int cameraX = this->camera->getX();
    int cameraY = this->camera->getY();

    this->bg->render(0 - cameraX, 0 - cameraY);

    int size = this->planetArray.size();
    for (int i = 0; i < size; i++)
        this->planetArray[i]->render(cameraX, cameraY);

    this->earth->render(cameraX, cameraY);
    this->moon->render(cameraX, cameraY);

    if (!(this->ufo->isDead()))
        this->ufo->render(cameraX, cameraY);

    if (!(this->ship->isDead()))
        this->ship->render(cameraX, cameraY);

    this->explosion->render(cameraX, cameraY);

    this->lifeBar->render(10, 10);
    this->lifeBarText->render();

//    this->cenario->show(cameraX, cameraY);

    if (this->isPaused)
    {
        this->pausedTitle->render();
        this->pausedMenu->render();
    }

    // Must always be on top
    this->console->render();
}
void GameStateGame::checkCollision()
{
    if (!(this->ufo) || !(this->ship))
        return;

    this->checkPlanets();

    if (this->ship->collidedWith(this->earth))
    {
        this->ufo->damage(666);
        this->game_over = true;
    }

    if (this->ship->isDead())
    {
        this->explosion->explodeAt(this->ship->getX(),
                                   this->ship->getY());
        delete this->ship;
        this->ship = NULL;

        this->game_over = true;
    }
    if (this->ufo->isDead())
    {
        this->explosion->explodeAt(this->ufo->getX(),
                                   this->ufo->getY());
        delete this->ufo;
        this->ufo = NULL;
    }
}
void GameStateGame::addPlanet()
{
    int hit = SDL::randomNumberBetween(1, 20);
    int x   = SDL::randomNumberBetween(0, Window::width  - 1);
    int y   = SDL::randomNumberBetween(0, Window::height - 1);

    PlanetRed* planet = new PlanetRed((this->planetSprite),
                                      hit,
                                      x + this->camera->getX(),
                                      y + this->camera->getY(),
                                      this->planetSprite->getWidth(),
                                      this->planetSprite->getHeight());

    this->planetArray.push_back(planet);
    Log::debug("GameStateGame::addPlanet Successful");
}
void GameStateGame::checkPlanets()
{
    int size = this->planetArray.size();

    for (int i = 0; i < size; i++)
    {
        if (this->ship->collidedWith(this->planetArray[i]))
        {
            this->explosion->explodeAt(this->planetArray[i]->getX(),
                                       this->planetArray[i]->getY());
            this->planetArray[i]->damage(666);
            this->ship->damage(1);
            this->lifeBar->decrease(1);
        }

        if (this->planetArray[i]->isDead())
        {
            delete this->planetArray[i];
            this->planetArray.erase(this->planetArray.begin() + i);
            Log::debug("GameStateGame::checkPlanets Delete Planet no " +
                       SDL::intToString(i));
        }
    }
}
void GameStateGame::processEvents()
{
    InputManager* input = InputManager::getInstance();
    input->update(this->camera->getX(), this->camera->getY());

    this->will_quit = input->quitRequested();

    if (console->isVisible())
        return;

    if ((input->isKeyDown(SDLK_ESCAPE)) ||
        (input->isKeyDown(SDLK_q)))
        this->will_quit = true;

    if (input->isKeyDown(SDLK_p))
        this->isPaused?
            this->isPaused = false:
            this->isPaused = true;

    // The only things we're going to care when the game is
    // paused are quitting, unpausing and the pause menu.
    if (this->isPaused)
    {
        if (this->pausedMenu->optionWasSelected())
        {
            switch (this->pausedMenu->getSelectedOption())
            {
            case 0:
                this->isPaused = false;
                break;
            case 1:
                this->will_quit = true;
                break;
            default:
                break;
            }
        }
        return;
    }

    if ((input->isKeyDown(SDLK_F2)))
        this->game_over = true;

    if (input->isKeyDown(SDLK_f))
        Window::toggleFullscreen();

    // These are extra keys that I've added for teh lulz

    // Toggle bounding boxes
    if (input->isKeyDown(SDLK_F1))
        (GameStateGame::showBoundingBoxes)?
            GameStateGame::showBoundingBoxes = false:
            GameStateGame::showBoundingBoxes = true;

    // Reloading config file at runtime!
    if (input->isKeyDown(SDLK_r))
        Config::reload();

    if (input->isMouseDown(SDL_BUTTON_LEFT))
        this->addPlanet();

    // Empty ufo command list
    if (input->isMouseDown(SDL_BUTTON_MIDDLE))
    {
        if (this->ufo)
            this->ufo->empty();
    }

    if (input->isMouseDown(SDL_BUTTON_RIGHT))
    {
        if (this->ufo)
            this->ufo->enqueueCommand(input->getMouseX(),
                                      input->getMouseY());
    }

    if (input->isKeyDown(SDLK_F3))
        Window::center();

    if (input->isKeyDown(SDLK_F4))
        Window::setPosition(10, 10);
}

