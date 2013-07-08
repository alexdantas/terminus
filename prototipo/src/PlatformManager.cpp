#include "PlatformManager.hpp"
#include "GameObject.hpp"

PlatformManager::PlatformManager(Rectangle gameArea, float maxHeight):
    container(NULL),
    gameArea(gameArea),
    maxHeight(maxHeight),
    minHeight(20),
    topPlatform(NULL)
{
    this->container = new PlatformContainer(30, gameArea);

//    this->container->addAll();
}
PlatformManager::~PlatformManager()
{
    if (this->container)
        delete this->container;
}
void PlatformManager::setFrequency(int frequency)
{
    this->maxHeight = frequency;
}
void PlatformManager::update(float dt)
{
    // Check inclusion rules

    // whatever

    this->container->update(dt);
}
void PlatformManager::add()
{
    float bottomLimit;
    float upperLimit;

    if (this->topPlatform)
        bottomLimit = this->topPlatform->getY() - this->minHeight;
    else
        bottomLimit = this->gameArea.bottom - (this->minHeight * 3);

    upperLimit = bottomLimit - this->maxHeight;
    // TEST IF OUTTA SCREEN

    // This will add a random-type platform between the
    // currently allowed limits.
    //
    // They are:
    // * Screen width
    // * Platform height size.

    Point a(this->gameArea.leftmost,  bottomLimit);
    Point b(this->gameArea.rightmost, upperLimit);
    this->container->addBetween(a, b);

    this->topPlatform = this->container->getTopPlatform();
}
void PlatformManager::addAll()
{
    while (!(this->container->isFull()))
        this->add();
}
void PlatformManager::render(float cameraX, float cameraY)
{
    // If I want to render some stats, whatever

    this->container->render(cameraX, cameraY);
}

