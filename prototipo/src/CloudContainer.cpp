#include "CloudContainer.hpp"
#include "SDL.hpp"

CloudContainer::CloudContainer(unsigned int maxAmmount, Sprite* sprite):
    maxAmmount(maxAmmount),
    currentAmmount(0),
    sprite(sprite)
{
    this->indirection.resize(this->maxAmmount);

    for (unsigned int i = 0; i < (this->maxAmmount); i++)
    {
        Cloud* tmp = new Cloud(0, 0, 0.2, this->sprite, i);
        this->container.push_back(tmp);
    }
}
CloudContainer::~CloudContainer()
{
    for (unsigned int i = 0; i < (this->maxAmmount); i++)
    {
        if (this->container[i])
            delete this->container[i];
    }
}
void CloudContainer::addAt(Point p)
{
    Cloud* tmp = this->container[this->currentAmmount];

    tmp->setPosition(p);

//    float speed = ((float)(SDL::randomNumberBetween(1, 10)))/10.0;
    float speed = 0.1;
    tmp->setSpeed(speed);

    tmp->setVisible(true);

    this->currentAmmount++;
}
void CloudContainer::update(uint32_t dt)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
    {
        if ((this->container[i]) &&
            (this->container[i]->isVisible()))
            this->container[i]->update(dt);
    }
}
void CloudContainer::render(float cameraX, float cameraY)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
    {
        if ((this->container[i]) &&
            (this->container[i]->isVisible()))
            this->container[i]->render(cameraX, cameraY);
    }
}

