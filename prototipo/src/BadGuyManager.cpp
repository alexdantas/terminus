#include "BadGuyManager.hpp"

BadGuyManager::BadGuyManager(unsigned int ammout, Rectangle *gameArea, PlatformManager *platforms):
    container(NULL),
    maxHeight(maxHeight),
    minHeight(20),
    currentCameraY(0)
{
    this->container = new BadGuyContainer(ammout, gameArea, platforms);

}
BadGuyManager::~BadGuyManager()
{
    if (this->container)
        delete this->container;
}
void BadGuyManager::update(float dt)
{
    this->container->update(30);
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

