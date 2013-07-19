#include "CloudManager.hpp"
#include "GameObject.hpp"

CloudManager::CloudManager(Rectangle gameArea, int maxAmmount):
    container(NULL),
    gameArea(gameArea),
    timer(500)
{
    this->container = new CloudContainer(maxAmmount, gameArea);

    this->timer.startCounting();
}
CloudManager::~CloudManager()
{
    if (this->container)
        delete this->container;
}
void CloudManager::setDelay(int delay)
{
    this->timer.setDelay(delay);
}
void CloudManager::setArea(Rectangle area)
{
    this->container->limitArea(area);
}
void CloudManager::update(float dt)
{
    if (this->timer.isDone())
    {
        this->container->addAtRandom();
        this->timer.startCounting();
    }

    this->container->update(dt);
}
void CloudManager::render(float cameraX, float cameraY)
{
    this->container->render(cameraX, cameraY);
}
void CloudManager::add()
{
    this->container->addAtRandom();
}
void CloudManager::addAll()
{
    this->container->addAll();
}

