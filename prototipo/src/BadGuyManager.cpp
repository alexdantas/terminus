#include "BadGuyManager.hpp"

BadGuyManager::BadGuyManager(unsigned int ammout, Rectangle *gameArea, PlatformManager *platforms):
    container(NULL),
    maxHeight(maxHeight),
    minHeight(20),
    currentCameraY(0)
{
    this->container = new BadGuyContainer(ammout, gameArea, platforms);

    this->venusTimer = new TimerCounter(1500);
    this->venusTimer->startCounting();
}
BadGuyManager::~BadGuyManager()
{
    if (this->container)
        delete this->container;
}
void BadGuyManager::update(float dt)
{
    if (this->venusTimer->isDone())
    {
        this->venusTimer->startCounting();
        if (this->container->venusCount < 5)
            if (SDL::randomBool()) // adding a nice ammount of randomness
                this->container->addVenus();
    }

//    this->container->update(30);
    this->container->update(dt);
}
void BadGuyManager::render(float cameraX, float cameraY)
{
    this->currentCameraY = cameraY;

    this->container->render(cameraX, cameraY);
}
std::vector<BadGuy*> BadGuyManager::getBadGuys()
{
    return this->container->badguy;
}

