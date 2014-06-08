#include <iostream> // NULL
#include "GameObject.hpp"

// Almost a purely virtual class

GameObject::GameObject(float x, float y, int w, int h):
    position(NULL),
    box(NULL)
{
    this->position = new Point(x, y);
    this->box      = new Rectangle(x, y, w, h);

}
GameObject::~GameObject()
{
    if (this->position) delete this->position;
    if (this->box) delete this->box;
}
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
    return this->box->center.x;
}
float GameObject::getCenterY()
{
    return this->box->center.y;
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
    // return (this->boundingBox->overlaps(other->boundingBox));
    return (this->box->overlaps(other->box));
}
// bool GameObject::oneWayCollidedWith(GameObject* other)
// {
//     // if (this->boundingBox->overlaps(other->boundingBox))
//     // {
//     //     if (!(this->previousBoundingBox))
//     //         return false;

//     //     if (this->previousBoundingBox->bottom < other->boundingBox->top)
//     //         return true;
//     //     else
//     //         return false;
//     // }

//     return false;
// }
void GameObject::scaleBoundingBox(float scaleX, float scaleY)
{
    this->box->stretch(scaleX, scaleY);
}

