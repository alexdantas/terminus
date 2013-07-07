#include "PlatformManager.hpp"
#include "GameObject.hpp"

PlatformManager::PlatformManager(Rectangle gameArea):
    container(NULL)
{
    this->container = new PlatformContainer(30, gameArea);

    this->container->addAll();
}
PlatformManager::~PlatformManager()
{
    if (this->container)
        delete this->container;
}
void PlatformManager::update(float dt)
{
    // Check inclusion rules

    // whatever


    this->container->update(dt);
}
void PlatformManager::render(float cameraX, float cameraY)
{
    // If I want to render some stats, whatever

    this->container->render(cameraX, cameraY);
}

