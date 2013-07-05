#include "Platform.hpp"
#include "GameStateGame.hpp"

Platform::Platform(Sprite* sprite, float x, float y, int w, int h, unsigned int id):
    GameObject(x, y, w, h),
    id(id),
    sprite(sprite),
    visible(true)
{ }
Platform::~Platform()
{ }
void Platform::render(float cameraX, float cameraY)
{
    this->sprite->render(this->box->x - cameraX,
                         this->box->y - cameraY);

    if (GameStateGame::showBoundingBoxes)
        this->boundingBox->render(cameraX, cameraY);
}
void Platform::update(uint32_t dt)
{
    // yeah, regular platforms just sit there, doing nothing
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

