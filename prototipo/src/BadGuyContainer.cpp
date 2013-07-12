#include "BadGuyContainer.hpp"

BadGuyContainer::BadGuyContainer(unsigned int maxAmmount) :
    maxAmmount(maxAmmount),
    currentAmmount(0)
 {
    this->badguy.resize(BadGuy::MAX); // Maximum size of the vector

    this->badguy[BadGuy::GRIFFIN] = new BadGuyGriffin(0, 6000-500, 415, 200, 20, Config::playerAcceleration);
    this->badguy[BadGuy::VENUS] = new BadGuyVenus(0, 6000-390, 61, 391, 20, Config::playerAcceleration);

    this->badguy[BadGuy::GRIFFIN]->setHorizontalLimit(0, 800);
    this->badguy[BadGuy::GRIFFIN]->setVerticalLimit(5000, 6000);
 }

BadGuyContainer::~BadGuyContainer()
 {
    unsigned int size = this->badguy.size();
    for (unsigned int i = 0; i < size; i++)
        if (this->badguy[i])
            delete (this->badguy[i]);
 }

void BadGuyContainer::add(Point p, BadGuy::BadGuyType type)
 {
     //TO DO
 }

void BadGuyContainer::render(float cameraX, float cameraY)
 {
     unsigned int size = this->badguy.size();
    for (unsigned int i = 0; i < size; i++)
        if (this->badguy[i])
           this->badguy[i]->render(cameraX, cameraY);
 }

void BadGuyContainer::update(float dt)
{
    unsigned int size = this->badguy.size();
    for (unsigned int i = 0; i < size; i++)
        if (this->badguy[i] && this->badguy[i]->isVisible())
        {
            this->badguy[i]->update(dt);
            this->badguy[i]->commitMovement();
        }
}
