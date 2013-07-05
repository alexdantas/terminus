#include "GameStateGame.hpp"
#include "StateManager.hpp"
#include "Config.hpp"
#include "Log.hpp"
#include "Window.hpp"
#include "LoadingScreen.hpp"
#include "PhysicsManager.hpp"

bool GameStateGame::showBoundingBoxes = false;

// All possible commands the user might type into the console
enum GameStateGameCommands
{
    COMMAND_QUIT, COMMAND_ADD_PLATFORM, COMMAND_INVERT_GRAVITY,
    COMMAND_FLY,  COMMAND_ADD_CLOUD
};

GameStateGame::GameStateGame():
    bg(NULL),
    will_quit(false),
    will_return_to_main_menu(false),
    game_over(false),
    camera(NULL),
    bgmusic(NULL),
    apterus(NULL),
    lifeBar(NULL),
    lifeBarFont(NULL),
    lifeBarText(NULL),
    isPaused(false),
    font(NULL),
    pausedTitle(NULL),
    console(NULL),
    platforms(NULL),
    cloudContainer(NULL)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
    UNUSED(stack);

    LoadingScreen loading("loading...", "ttf/LithosProRegular.ttf");

    loading.increase(2);

    this->bg = new Sprite("img/fundo.png");

    loading.increase(30);

    this->camera = new Camera(0, 0,
                              Window::width, Window::height,
                              Config::cameraScrollSpeed);
    this->camera->lockXAxis();
    this->camera->setVerticalLimit(0, this->bg->getHeight());

    loading.increase(3);

    int playerX = 245;
    int playerY = this->bg->getHeight() - 201;

    this->apterus = new Player(playerX, playerY,
                               245, 200,
                               20,
                               Config::playerAcceleration);
    this->apterus->setHorizontalLimit(0, this->bg->getWidth());
    this->apterus->setVerticalLimit(0, this->bg->getHeight());

    loading.increase(10);

    this->lifeBar = new ProgressBar(200, 20, this->apterus->getHitpoints(), this->apterus->getHitpoints());
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


    this->bgmusic = new Music("ogg/escaping.ogg");
    this->bgmusic->play();

    loading.increase(10);

    this->consoleFont = new Font("ttf/UbuntuMono.ttf", 18);

    this->console = new DropDownConsole(this->consoleFont, 0, 0, Window::width, Window::height/3);
    this->console->setPromptForegroundColor(Color(20, 230, 20));
    this->console->setForegroundColor(Color(20, 180, 20));
    this->console->setBackgroundColor(Color(20, 20, 20, 210));
    this->console->setScrollSpeed(1.78);
    this->console->refresh();
    this->console->print("* Game console loaded *");

    this->console->addCommand("quit", COMMAND_QUIT);
    this->console->addCommand("add", COMMAND_ADD_PLATFORM);
    this->console->addCommand("addcloud", COMMAND_ADD_CLOUD);
    this->console->addCommand("toinfinityandbeyond", COMMAND_FLY);
    this->console->addCommand("whocaresaboutphysics", COMMAND_INVERT_GRAVITY);

    loading.increase(6);

    this->platforms = new PlatformManager(Rectangle(0, 0, this->bg->getWidth(), this->bg->getHeight()));

    loading.increase(3);

    Rectangle cloudLimit(0,
                         this->bg->getHeight() - Window::height,
                         Window::width,
                         Window::height);

    this->cloudContainer = new CloudContainer(Config::cloudsLimit, cloudLimit);
    this->cloudContainer->addAt(Point(this->bg->getWidth()/4,
                                      this->bg->getHeight() - 400));

    this->cloudContainer->addAtRandom();
    this->cloudContainer->addAtRandom();
    this->cloudContainer->addAtRandom();

    loading.increase(10);
}
int GameStateGame::unload()
{
    // Before deleting everything, we must see if we
    // just won.
    // If we did, then this->ufo will be NULL
    bool we_won = false;

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

    safe_delete(this->bg);
    safe_delete(this->bgmusic);
    safe_delete(this->camera);


    safe_delete(this->apterus);

    safe_delete(this->font);
    safe_delete(this->pausedTitle);

    safe_delete(this->lifeBar);
    safe_delete(this->lifeBarFont);
    safe_delete(this->lifeBarText);

    if (we_won)
        return 1;
    else
        return 0;
}
int GameStateGame::update(uint32_t dt)
{
    if (this->will_quit)
        return GameState::QUIT;

    if (this->will_return_to_main_menu)
        return GameState::MAIN_MENU;

    // Player/UFO has died.
    // Will only go to the GAME OVER screen when the
    // explosion ends.
    if (this->game_over)
        return GameState::GAME_OVER;

    this->processEvents();

    this->console->update(dt);

    int command = this->console->hasCommand();
    if (command)
    {
        switch (this->console->getCommand())
        {
        case COMMAND_QUIT:
            this->console->print("Quitting...");
            this->will_quit = true;
            break;

        // case COMMAND_ADD_PLATFORM:
        // {
        //     // Will add `n` platforms, being `n` the number passed
        //     // as an argument to the command `add`.
        //     int platformAmmount = 1;

        //     // Args start counting from 1 - the command itself.
        //     // If it's greater than 1 then we have an argument.
        //     if (this->console->getCommandArgsAmmount() > 1)
        //         platformAmmount = SDL::stringToInt(this->console->getCommandArg(1));

        //     for (int i = 0; i < platformAmmount; i++)
        //         this->platforms->addBetween(Point(0, 0),
        //                                     Point(this->bg->getWidth(),
        //                                           this->bg->getHeight()));
        // }
        //     break;

        case COMMAND_ADD_CLOUD:
        {
            // Will add `n` clouds, being `n` the number passed
            // as an argument to the command `add`.
            int cloudAmmount = 1;

            // Args start counting from 1 - the command itself.
            // If it's greater than 1 then we have an argument.
            if (this->console->getCommandArgsAmmount() > 1)
                cloudAmmount = SDL::stringToInt(this->console->getCommandArg(1));

            for (int i = 0; i < cloudAmmount; i++)
                this->cloudContainer->addAtRandom();
        }
            break;

        case COMMAND_FLY:
            this->apterus->toggleFlyMode();
            break;

        case COMMAND_INVERT_GRAVITY:
            PhysicsManager::gravityAcceleration *= -1;
            break;

        default:
            this->console->print("Unknown Command!");
            break;
        }
    }
    if (this->isPaused)
    {
        return GameState::CONTINUE;
    }

    // Things that are up there updates independently if the
    // game's paused.
    // From now on, they won't.

    if (this->apterus)
        this->apterus->update(dt);

    this->camera->update(dt);

    float cameraLowestPoint = this->camera->getY() + this->camera->getHeight();
    float cameraScrollPoint = cameraLowestPoint    - this->camera->getHeight()/3;
    UNUSED(cameraScrollPoint);

    this->camera->centerOn(this->apterus->getCenterX(),
                           this->apterus->getCenterY());

//    if (cameraLowestPoint > levelHeight)
//        this->camera->setY(levelHeight - this->camera->getHeight());

//     // Relative-stuff
//     float cameraOffset  = this->camera->getY();
//     float scrollPoint   = 200; // relative to the camera
//     float playerCenterY = (this->apterus->getY() + this->apterus->getHeight()/2) - cameraOffset;

//     if (playerCenterY < scrollPoint)
//         this->camera->scroll(Camera::UP, (playerCenterY - cameraOffset));
// //        this->camera->setY(playerCenterY);

//     // this->camera->centerOn(this->apterus->getX() + (this->apterus->getWidth()/2),
//     //                        this->apterus->getY() + (this->apterus->getHeight()/2));

//     // Global stuff
//     float cameraLowestPoint = cameraOffset + this->camera->getY();
//     float levelLowestPoint  = this->bg->getHeight();

//     if (cameraLowestPoint > levelLowestPoint)
//         this->camera->setY(levelLowestPoint - cameraOffset);

    if (this->apterus->getY() >= cameraLowestPoint)
    {
        // this is where the player dies
    }

    this->cloudContainer->update(dt);
    this->checkCollision();

    return GameState::CONTINUE;
}
void GameStateGame::render()
{
    int cameraX = this->camera->getX();
    int cameraY = this->camera->getY();

    Window::clear();
    this->bg->render(0 - cameraX, 0 - cameraY);

    this->cloudContainer->render(cameraX, cameraY);

    // This is a BAD HACK to make sure the clouds are always added at
    // the camera.
    // Must find a way to always limit the area to the camera!
    this->cloudContainer->limitArea(Rectangle(cameraX, cameraY,
                                              Window::width, Window::height));

    this->platforms->render(cameraX, cameraY);

    if (!(this->apterus->isDead()))
        this->apterus->render(cameraX, cameraY);

    if (this->isPaused)
    {
        this->pausedTitle->render();
    }

    this->lifeBar->render(10, 10);
    this->lifeBarText->render();

    if (Config::debugMode)
    {
        Text debug(this->lifeBarFont);

        // Showing the gravity
        // (ugly hack)
        debug.setText("Gravity: " +
                      SDL::intToString(PhysicsManager::gravityAcceleration) +
                      "." +
                      SDL::intToString(abs(PhysicsManager::gravityAcceleration * 10)));
        debug.setPosition(0, 40);
        debug.render();


    }


    // Must always be on top
    this->console->render();
}
void GameStateGame::checkCollision()
{
    if (!(this->apterus))
        return;

//     if (this->platforms->collidesWith(this->apterus))
//     {
//         this->apterus->undoUpdate();
//         this->apterus->jump(false);
//     }
//     else
//     {
//         // TODO
//         // what should I do to make the player fall through
//         // air without forcing him to the ground?
// //        this->apterus->fall();
//     }

    if (this->apterus->isDead())
    {
        delete this->apterus;
        this->apterus = NULL;

        this->game_over = true;
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

    if ((input->isKeyDown(SDLK_m)))
        this->will_return_to_main_menu = true;

    if (input->isKeyDown(SDLK_p))
        this->isPaused?
            this->isPaused = false:
            this->isPaused = true;

    // The only things we're going to care when the game is
    // paused are quitting and unpausing

    if ((input->isKeyDown(SDLK_F2)))
        this->game_over = true;

    if (input->isKeyDown(SDLK_f))
        Window::toggleFullscreen();

    // These are extra keys that I've added for teh lulz

    // Toggle bounding boxes
    if (input->isKeyDown(SDLK_F1))
        (Config::showBoundingBoxes)?
            Config::showBoundingBoxes = false:
            Config::showBoundingBoxes = true;

    // Reloading config file at runtime!
    if (input->isKeyDown(SDLK_r))
        Config::reload();

    if (input->isKeyDown(SDLK_F3))
        Window::center();

    if (input->isKeyDown(SDLK_F4))
        Window::setPosition(10, 10);
}

