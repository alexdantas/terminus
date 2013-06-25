#include "GameObject.hpp"

// Almost a purely virtual class

GameObject::GameObject(float x, float y, int w, int h):
    box(NULL),
    boundingBox(NULL),
    previousBoundingBox(NULL)
{
    this->box = new Rectangle(x, y, w, h);

    this->boundingBox = new RectBoundingBox(this->box);
}
GameObject::~GameObject()
{
    if (this->boundingBox) delete this->boundingBox;
    if (this->box)         delete this->box;
}

// TODO create a way of changing the bounding box relative to
//       the object.
//       The problem is that RectBoundingBox DEPENDS on rect
//
// void GameObject::setBoundingBox(int x, int y, int w, int h)
// {
//     // remember: x/y are relative to the object!

// }

float GameObject::getX()
{
    return this->box->x;
}
float GameObject::getY()
{
    return this->box->y;
}
float GameObject::getCenterX()
{
    return this->box->centerX;
}
float GameObject::getCenterY()
{
    return this->box->centerY;
}
int GameObject::getWidth()
{
    return this->box->w;
}
int GameObject::getHeight()
{
    return this->box->h;
}
bool GameObject::collidedWith(GameObject* other)
{
    return (this->boundingBox->overlaps(other->boundingBox));
}
bool GameObject::oneWayCollidedWith(GameObject* other)
{
    if (this->boundingBox->overlaps(other->boundingBox))
    {
        if (!(this->previousBoundingBox))
            return false;

        if (this->previousBoundingBox->bottom < other->boundingBox->top)
            return true;
        else
            return false;
    }

    return false;
}

