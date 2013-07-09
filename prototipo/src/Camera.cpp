#include "Camera.hpp"

Camera::Camera(float x, float y, int w, int h):
    x(x),
    y(y),
    w(w),
    h(h),
    lockedX(false),
    lockedY(false),
    hasVerticalLimit(false),
    topLimitY(0),
    bottomLimitY(0)
{ }
float Camera::getX()
{
    return this->x;
}
float Camera::getY()
{
    return this->y;
}
int Camera::getWidth()
{
    return this->w;
}
int Camera::getHeight()
{
    return this->h;
}
void Camera::setX(float x)
{
    this->x = x;
}
void Camera::setY(float y)
{
    this->y = y;
}
void Camera::setVerticalLimit(int top, int bottom)
{
    this->topLimitY    = top;
    this->bottomLimitY = bottom;

    this->hasVerticalLimit = true;
}
void Camera::centerOn(int x, int y)
{
    if (!(this->lockedX))
        this->x = (x - this->w/2);

    if (!(this->lockedY))
        this->y = (y - this->h/2);

    // Kinda intuitive, right?
    // If it has any kind of limit, make it so

    if (this->hasVerticalLimit)
    {
        if (this->y < this->topLimitY)
            this->y = this->topLimitY;

        if ((this->y + this->h) > this->bottomLimitY)
            this->y = (this->bottomLimitY - this->h);
    }
}
void Camera::lockXAxis(bool option)
{
    this->lockedX = option;
}

// NOTE BUG WARNING HACK
//
// Look, these functions are not working and should be ignored.
// I'm only keeping them here because they might be of some use
// on the future.

// void Camera::setScrollSpeed(float speed)
// {
//     this->scrollSpeed = speed;
// }
// void Camera::increaseScrollSpeedBy(float acc)
// {
//     this->scrollSpeed += acc;
// }
// void Camera::decreaseScrollSpeedBy(float acc)
// {
//     this->scrollSpeed -= acc;
// }
// void Camera::scroll(Direction dir, float ammount)
// {
//     float scrollAmmount;

//     // ammount > 0?
//     //     scrollAmmount = ammount:
//     //     scrollAmmount = this->scrollSpeed;
//     scrollAmmount = ammount;
//     switch (dir)
//     {
//     case LEFT:  this->speedX -= scrollAmmount; break;
//     case RIGHT: this->speedX += scrollAmmount; break;
//     case UP:    this->speedY -= scrollAmmount; break;
//     case DOWN:  this->speedY += scrollAmmount; break;
//     }
// }
// void Camera::unscroll(Direction dir)
// {
//     switch (dir)
//     {
//     case LEFT:  this->speedX += this->scrollSpeed; break;
//     case RIGHT: this->speedX -= this->scrollSpeed; break;
//     case UP:    this->speedY += this->scrollSpeed; break;
//     case DOWN:  this->speedY -= this->scrollSpeed; break;
//     }
// }
// void Camera::stop()
// {
//     this->speedX = 0;
//     this->speedY = 0;
// }

