#include "Platform.hpp"
#include "GameStateGame.hpp"

Platform::Platform(Sprite* sprite, float x, float y, int w, int h, PlatformType type):
    GameObject(x, y, w, h),
    type(type),
    sprite(sprite),
    visible(true),
    occupied(false)
{ }
Platform::~Platform()
{ }
void Platform::render(float cameraX, float cameraY)
{
    if (this->visible)
        this->sprite->render(this->box->x - cameraX,
                             this->box->y - cameraY);

    // if (Config::showBoundingBoxes)
    //     this->boundingBox->render(cameraX, cameraY);
}
void Platform::update(float dt)
{
    // Yeah, regular platforms just sit there, doing nothing
    // Maybe when I implement movable platforms things will get
    // more exciting.
    UNUSED(dt);
}
void Platform::setPosition(Point p)
{
    this->box->setX(p.x);
    this->box->setY(p.y);
}
bool Platform::isVisible()
{
    return (this->visible);
}
void Platform::setVisible(bool option)
{
    this->visible = option;
}

