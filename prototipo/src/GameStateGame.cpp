#include "GameStateGame.hpp"
#include "StateManager.hpp"
#include "Config.hpp"
#include "Log.hpp"
#include "Window.hpp"
#include "LoadingScreen.hpp"
#include "Graphics.hpp"
#include "PhysicsManager.hpp"
#include "PlatformVanishing.hpp" // whoa, do we really need this?

// All possible commands the user might type into the console
enum GameStateGameCommands
{
    COMMAND_QUIT,     COMMAND_ADD_PLATFORM, COMMAND_INVERT_GRAVITY,
    COMMAND_FLY,      COMMAND_ADD_CLOUD,    COMMAND_HELP,
    COMMAND_CONTROLS, COMMAND_GIVE_UP
};

GameStateGame::GameStateGame():
    bg(NULL),
    will_quit(false),
    will_return_to_main_menu(false),
    game_over(false),
    camera(NULL),
    timer(NULL),
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
    cloudContainer(NULL),
    fadeOut(NULL)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
    UNUSED(stack);

    LoadingScreen loading("loading...");
    loading.setBg("img/loading2.png");
    loading.increase(0);

    loading.setSubtitle("Loading background...");
    this->bg = new Sprite("img/fundo.png");

    loading.increase(30);

    loading.setSubtitle("Loading camera...");
    this->camera = new Camera(0, 0,
                              Window::width, Window::height
                              );
    this->camera->lockXAxis();
    this->camera->setVerticalLimit(0, this->bg->getHeight());

    this->timer = new TimerCounter(5000);

    loading.setSubtitle("Loading player...");
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
    this->lifeBarText->setText("Life Points");
    this->lifeBarText->setPosition(10, 10);

    loading.setSubtitle("Loading font...");
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

    loading.setSubtitle("Loading dropdown console...");
    loading.increase(10);

    this->consoleFont = new Font("ttf/UbuntuMono.ttf", 18);

    this->console = new DropDownConsole(this->consoleFont, 0, 0, Window::width, Window::height/3);
    this->console->setPromptForegroundColor(Color(20, 230, 20));
    this->console->setForegroundColor(Color(20, 180, 20));
    this->console->setBackgroundColor(Color(20, 20, 20, 210));
    this->console->setScrollSpeed(1000);
    this->console->refresh();
    this->console->print("* Game console loaded *");

    this->console->addCommand("quit", COMMAND_QUIT);
    this->console->addCommand("add", COMMAND_ADD_PLATFORM);
    this->console->addCommand("addcloud", COMMAND_ADD_CLOUD);
    this->console->addCommand("toinfinityandbeyond", COMMAND_FLY);
    this->console->addCommand("whocaresaboutphysics", COMMAND_INVERT_GRAVITY);

    loading.increase(6);

    loading.setSubtitle("Loading platforms...");
    // The area that platforms will be spawned
    // (will cut a little from the top)
    Rectangle gameArea(0, 300, this->bg->getWidth(), this->bg->getHeight() - 300);

    this->platforms = new PlatformManager(gameArea, (Config::playerJump * 5));

    this->badguy = new BadGuyManager(10);

    loading.increase(3);

    Rectangle cloudLimit(0,
                         this->bg->getHeight() - Window::height,
                         Window::width,
                         Window::height);

    this->cloudContainer = new CloudContainer(Config::cloudsLimit, cloudLimit);

    loading.increase(10);

    this->fadeOut = new Fade(Fade::FADE_OUT, 1000);
}
int GameStateGame::unload()
{

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
    safe_delete(this->badguy);

    safe_delete(this->font);
    safe_delete(this->pausedTitle);

    safe_delete(this->lifeBar);
    safe_delete(this->lifeBarFont);
    safe_delete(this->lifeBarText);
    safe_delete(this->fadeOut);

    if (we_won)
        return 1;
    else
        return 0;
}
GameState::StateCode GameStateGame::update(float dt)
{
    // If we're trying to quit, the fade out effect will start.
    // When it's finished, we will actually quit the game.
    if (this->fadeOut->isDone())
        return GameState::QUIT;

    if (this->will_quit)
        this->fadeOut->start();

    if (this->will_return_to_main_menu)
        return GameState::MAIN_MENU;

    // If we're quitting, this will update the fade out effect.
    // If not, will do nothing.
    this->fadeOut->update(dt);

    // Player has died.
    if (this->game_over){
        if(this->timer->isDone())
            return GameState::GAME_OVER;
    }

    this->updateInput();

InputManager* input = InputManager::getInstance();
    if (input->isKeyDown(SDLK_t))
        return GameState::GAME_OVER;

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
            if(this->apterus)
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
        return GameState::CONTINUE;

    // Things that are up there updates independently if the
    // game's paused.
    // From now on, they won't.

    // Will react to any object that's above movable platforms.
    this->checkPlatforms();

    // Will update all platforms.
    this->platforms->update(dt);

    if (this->apterus)
    {
        this->apterus->update(dt);
        this->camera->centerOn(this->apterus->getCenterX(),
                           this->apterus->getCenterY());
    }
    this->badguy->update(dt);


    // if (this->apterus->getY() >= cameraLowestPoint)
    // {
    //     // this is where the player dies
    // }

    this->cloudContainer->update(dt);
    this->checkCollisions();

    return GameState::CONTINUE;
}
void GameStateGame::render()
{
    int cameraX = this->camera->getX();
    int cameraY = this->camera->getY();

    this->bg->render(0 - cameraX, 0 - cameraY);

    this->cloudContainer->render(cameraX, cameraY);

    // This is a BAD HACK to make sure the clouds are always added at
    // the camera.
    // Must find a way to always limit the area to the camera!
    this->cloudContainer->limitArea(Rectangle(cameraX, cameraY,
                                              Window::width, Window::height));

    this->platforms->render(cameraX, cameraY);

    if (this->apterus && !(this->apterus->isDead()))
        this->apterus->render(cameraX, cameraY);

    this->badguy->render(cameraX, cameraY);

    if (this->isPaused)
    {
        this->pausedTitle->render();
    }

    if (Config::showBoundingBoxes)
    {
        if(this->apterus){
            Rectangle tmp(this->apterus->box->x - cameraX,
                          this->apterus->box->y - cameraY,
                          this->apterus->box->w,
                          this->apterus->box->h);

            Graphics::drawRectangle(tmp);
        }
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

    // Must always be on top of the game elements
    this->console->render();

    // Must always be on top of everything
    // Will render the fade out effect if we're quitting.
    this->fadeOut->render();
}
void GameStateGame::checkPlatforms()
{
    bool playerIsSteppingOnAnyMovablePlatform = false;

    // Will check if any of the moving platforms collide with the
    // player.
    // Also will check if the player collides with any vanishing
    // platforms.

    for (std::list<Platform*>::iterator it = this->platforms->container->platforms.begin();
         it != this->platforms->container->platforms.end();
         it++)
    {
        // And here we are certain that it is indeed a moving platform.
        if ((*it)->type == Platform::MOVABLE)
        {
            // THIS is very strange.
            // I need to _downcast_ PlatformMovable to Platform.
            //
            // Since not all Platform is a PlatformMovable, what
            // should I do to force this?
            //
            // Maybe my whole design is wrong.

            PlatformMovable* movable = dynamic_cast<PlatformMovable*>(*it);

            // dynamic_cast returns NULL if we can't dynamically cast the
            // object.
            // If that's the case, it can't be helped, let's just ignore it.
            if (!movable)
                continue;

            // Player's standing above this movable platform
            if (this->apterus && ((movable->box->top) == (this->apterus->box->bottom)))
            {
                this->apterus->stepIntoMovablePlatform(movable);
                playerIsSteppingOnAnyMovablePlatform = true;
            }
        }
        // Will tell them to vanish if the player's colliding with any
        // one of them.
        else if ((*it)->type == Platform::VANISHING)
        {
            // Same dynamic_cast and casting problems as above.

            PlatformVanishing* vanishing = dynamic_cast<PlatformVanishing*>(*it);
            if (!vanishing)
                continue;

            // Player's standing above this vanishing platform
            if (this->apterus && ((vanishing->box->top) == (this->apterus->box->bottom)))
                vanishing->vanish(); // simple, right?
        }
        else continue;
        // I've always wanted to do this `else continue`.
        // Isn't it cute?
    }
    if (this->apterus && !playerIsSteppingOnAnyMovablePlatform)
        this->apterus->stepIntoMovablePlatform(NULL);
}
void GameStateGame::checkCollisions()
{
    // TODO maybe remove this later?
    // COllisions should go independent of the player, I guess.
    if (!(this->apterus))
        return;

    // Will check if any of the platforms collide with the player
    for (std::list<Platform*>::iterator it = this->platforms->container->platforms.begin();
         it != this->platforms->container->platforms.end();
         it++)
    {
        if (this->apterus && this->apterus->desiredPosition->overlaps((*it)->box))
        {
            // One-way collision
            // Check if previously the player was above the platform
            if (this->apterus->box->bottom <= (*it)->box->top)
            {
                this->apterus->desiredPosition->placeOnTop((*it)->box);
                this->apterus->jump(false);
                break;
            }
        }
    }
    // unsigned int size = platform->usedPlatforms.size();
    // for (unsigned int i = 0; i < size; i++)
    // {
    //     if (this->apterus->desiredPosition->overlaps(platform->usedPlatforms[i]->box))
    //     {
    //         // One-way collision
    //         // Check if previously the player was above the platform
    //         if (this->apterus->box->bottom <= platform->usedPlatforms[i]->box->top)
    //         {
    //             this->apterus->desiredPosition->placeOnTop(platform->usedPlatforms[i]->box);
    //             this->apterus->jump(false);
    //             break;
    //         }
    //     }
    // }

    // We're allowing the player to move.

    std::vector<BadGuy*> badguys = this->badguy->getBadGuys();
    unsigned int size = badguys.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if(this->apterus && this->apterus->collidedWith(badguys[i])){
            this->apterus->dealDamage();
            this->apterus->damage(1);
            this->lifeBar->decrease(1);
        }
    }

    if(this->apterus)
        this->apterus->commitMovement();

    if (this->apterus && this->apterus->isDead())
    {
        this->timer->startCounting();
        delete this->apterus;
        this->apterus = NULL;

        this->game_over = true;
    }
}
void GameStateGame::updateInput()
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

