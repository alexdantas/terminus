#include "PlatformVanishing.hpp"

PlatformVanishing::PlatformVanishing(Sprite* sprite, float x, float y, int w, int h, int delay):
    Platform(sprite, x, y, w, h, Platform::VANISHING),
    timer(NULL),
    vanishing(false)
{
    this->timer = new TimerCounter(delay);
}
PlatformVanishing::~PlatformVanishing()
{
    if (this->timer) delete this->timer;
}
void PlatformVanishing::vanish()
{
    if (this->vanishing) return;

    this->timer->startCounting();
    this->vanishing = true;
}
void PlatformVanishing::update(float dt)
{
    if (this->timer->isDone())
    {
        this->setVisible(false);
        return;
    }

    if (this->vanishing)
    {
        this->sprite->setTransparency(125);
    }
}
bool PlatformVanishing::isVanishing()
{
    return (this->vanishing);
}

