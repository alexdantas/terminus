#include "PlatformManager.hpp"
#include "GameObject.hpp"

PlatformManager::PlatformManager(Rectangle gameArea, float maxHeight):
    container(NULL),
    gameArea(gameArea),
    maxHeight(maxHeight),
    minHeight(20),
    topPlatform(NULL),
    currentCameraY(0)
{
    this->container = new PlatformContainer(30, gameArea);

    for (int i = 0; i < 10; i++)
        this->add();
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
    if (this->topPlatform)
    {
        // Will keep adding platforms whenever the camera gets close
        // to the topmost one.
        //
        // When their difference is lower than an arbitrary value, let's
        // add another platform.

        float cameraDiff = (this->currentCameraY - this->topPlatform->getY());
        if (cameraDiff < 1000) // arbitrary
            this->add();
    }

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
    this->currentCameraY = cameraY;

    this->container->render(cameraX, cameraY);
}

