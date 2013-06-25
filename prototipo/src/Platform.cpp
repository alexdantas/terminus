#include "Platform.hpp"
#include "GameStateGame.hpp"

Platform::Platform(Sprite* sprite, float x, float y, int w, int h):
    GameObject(x, y, w, h),
    sprite(sprite)
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

