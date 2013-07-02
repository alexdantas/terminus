#include "Shapes.hpp"

Point::Point(float x, float y):
    x(x),
    y(y)
{ }

Point::Point():
    x(0),
    y(0)
{ }

Rectangle::Rectangle(float x, float y, int w, int h):
    x(x),
    y(y),
    w(w),
    h(h)
{
    this->recalculate();
}
void Rectangle::addX(float x)
{
    this->x += x;

    this->recalculate();
}
void Rectangle::addY(float y)
{
    this->y += y;

    this->recalculate();
}
void Rectangle::setX(float x)
{
    this->x = x;

    this->recalculate();
}
void Rectangle::setY(float y)
{
    this->y = y;

    this->recalculate();
}
void Rectangle::recalculate()
{
    this->top    = y;
    this->bottom = y + h;

    this->leftmost  = x;
    this->rightmost = x + w;

    this->center = Point(x + (w/2),
                         y + (h/2));

    this->topLeft     = Point(x,     y);
    this->topRight    = Point(x + w, y);
    this->bottomLeft  = Point(x,     y + h);
    this->bottomRight = Point(x + w, y + h);
}


Circle::Circle(float x, float y, int r):
    x(x),
    y(y),
    r(r)
{
    this->centerX = x + r;
    this->centerY = y + r;
}
void Circle::addX(float x)
{
    this->x       += x;
    this->centerX += x;
}
void Circle::addY(float y)
{
    this->y       += y;
    this->centerY += y;
}


