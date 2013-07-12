#include "BadGuyManager.hpp"

BadGuyManager::BadGuyManager(unsigned int ammout):
    container(NULL),
    maxHeight(maxHeight),
    minHeight(20),
    currentCameraY(0)
{
    this->container = new BadGuyContainer(10);

    //To do

    //for (int i = 0; i < 10; i++)
    //    this->add();
}
BadGuyManager::~BadGuyManager()
{
    if (this->container)
        delete this->container;
}

void BadGuyManager::update(float dt)
{
    this->container->update(dt);
}

void BadGuyManager::render(float cameraX, float cameraY)
{
    this->currentCameraY = cameraY;

    this->container->render(cameraX, cameraY);
}

void BadGuyManager::add()
{
    //TO DO
}

void BadGuyManager::addAll()
{
    //TO DO
}

std::vector<BadGuy*> BadGuyManager::getBadGuys()
{
    return this->container->badguy;
}
