#include "PlatformMovable.hpp"

PlatformMovable::PlatformMovable(Sprite* sprite, float x, float y, int w, int h, float speed):
    Platform(sprite, x, y, w, h, Platform::MOVABLE),
    speed(speed),
    direction(RIGHT),
    hasHorizontalLimit(true),
    hasVerticalLimit(false)
{ }
PlatformMovable::~PlatformMovable()
{ }
void PlatformMovable::update(float dt)
{
    this->previousBox.copy(this->box);

    int dir;
    this->direction == RIGHT?
        dir =  1:
        dir = -1;

    float vx = (dir * this->speed) * dt;
    this->box->addX(vx);

    // Limiting, if necessary
    if (this->hasHorizontalLimit)
    {
        if (this->box->x < this->leftmostLimitX)
        {
            this->box->x = this->leftmostLimitX;
            this->direction = RIGHT;
        }

        if ((this->box->x + this->box->w) > this->rightmostLimitX)
        {
            this->box->x = (this->rightmostLimitX - this->box->w);
            this->direction = LEFT;
        }

        this->box->update();
    }
    if (this->hasVerticalLimit)
    {
        if (this->box->y < this->topLimitY)
            this->box->y = this->topLimitY;

        if ((this->box->y + this->box->h) >= this->bottomLimitY)
            this->box->y = (this->bottomLimitY - this->box->h);

        this->box->update();
    }
}
void PlatformMovable::setHorizontalLimit(int left, int right)
{
    this->leftmostLimitX  = left;
    this->rightmostLimitX = right;

    this->hasHorizontalLimit = true;
}
void PlatformMovable::setVerticalLimit(int top, int bottom)
{
    this->topLimitY    = top;
    this->bottomLimitY = bottom;

    this->hasVerticalLimit = true;
}

