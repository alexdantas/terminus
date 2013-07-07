#include "Cloud.hpp"

Cloud::Cloud(float x, float y, float speed, Sprite* sprite, unsigned int id):
    GameObject(x, y, sprite->getWidth(), sprite->getHeight()),
    id(id),
    speedX(speed),
    sprite(sprite)
{ }
Cloud::~Cloud()
{ }
void Cloud::update(float dt)
{
    this->box->addX(this->speedX * dt);
}
void Cloud::render(float cameraX, float cameraY)
{
    this->sprite->render(this->box->x - cameraX,
                         this->box->y - cameraY);
}
void Cloud::setPosition(Point p)
{
    this->box->setX(p.x);
    this->box->setY(p.y);
}
void Cloud::setSpeed(float speed)
{
    this->speedX = speed;
}
bool Cloud::isVisible()
{
    return (this->visible);
}
void Cloud::setVisible(bool set)
{
    this->visible = set;
}

