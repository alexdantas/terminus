#include "Camera.hpp"

Camera::Camera(float x, float y, int w, int h, float scrollSpeed):
    x(x),
    y(y),
    w(w),
    h(h),
    speedX(0),
    speedY(0),
    scrollSpeed(scrollSpeed),
    lockedX(false),
    lockedY(false)
{
}
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
void Camera::update(uint32_t dt)
{
    this->x += (this->speedX) * dt;
    this->y += (this->speedY) * dt;
}
void Camera::centerOn(int x, int y)
{
    if (!(this->lockedX))
        this->x = x - this->w/2;

    if (!(this->lockedY))
        this->y = y - this->h/2;
}
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
void Camera::scroll(Direction dir, float ammount)
{
    float scrollAmmount;

    // ammount > 0?
    //     scrollAmmount = ammount:
    //     scrollAmmount = this->scrollSpeed;
    scrollAmmount = ammount;
    switch (dir)
    {
    case LEFT:  this->speedX -= scrollAmmount; break;
    case RIGHT: this->speedX += scrollAmmount; break;
    case UP:    this->speedY -= scrollAmmount; break;
    case DOWN:  this->speedY += scrollAmmount; break;
    }
}
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
void Camera::lockXAxis()
{
    this->lockedX = true;
}


