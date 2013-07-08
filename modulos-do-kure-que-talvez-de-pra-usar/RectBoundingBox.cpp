#include "RectBoundingBox.hpp"
#include "Graphics.hpp"

RectBoundingBox::RectBoundingBox(float x, float y, int w, int h):
    rect(NULL),
    externalRect(false)
{
    this->rect = new Rectangle(x, y, w, h);

    this->update();
}
RectBoundingBox::RectBoundingBox(Rectangle* rect):
    rect(NULL)
{
    this->rect = rect;

    this->externalRect = true;

    this->update();
}
RectBoundingBox::~RectBoundingBox()
{
    if (!(this->externalRect))
        if (this->rect)
            delete this->rect;
}
bool RectBoundingBox::overlaps(RectBoundingBox* other)
{
    if (!other)      return false;
    if (!this->rect) return false;

    // I tried to keep this as simple as possible.
    // But note that this method works on the negative:
    // we test if they've not collided.
    // If not, return true.

    Rectangle* A = this->rect;
    Rectangle* B = other->rect;

    // float diffX = A->x - B->x;
    // float diffY = A->y - B->y;

    // if ((diffX > B->dx) || ((-1 * diffX) > A->dx))
    //     return false;

    // if ((diffY > B->dy) || ((-1 * diffY) > A->dy))
    //     return false;

    // return true;

    // This is a very simple collision detection algorithm.
    // We have two rectangles (A and B), and we must see if they
    // DON'T intersect.
    //
    // That's summed up on this 4 rules:
    //
    // * A's bottom edge is higher than B's top edge;
    // * A's top edge is lower than B's bottom edge;
    // * A's left edge is to the right of B's right edge;
    // * A's right edge is to the left of B's left edge;

    // if (((A->dy) <= (B->y))  ||
    //     ((A->y)  >= (B->dy)) ||
    //     ((A->x)  >= (B->dx)) ||
    //     ((A->dx) <= (B->x)))
    //     return false;

    // return true;

    if ((A->x <= B->rightmost) &&
        (B->x <= A->rightmost) &&
        (A->y <= B->bottom) &&
        (B->y <= A->bottom))
        return true;

    return false;
}
    /// Tells if we've collided with #other circular thing.
// bool RectBoundingBox::overlaps(CircBoundingBox* other)
// {
//     // TODO
//     return false;
// }

// bool RectBoundingBox::overlaps(CircBoundingBox* other)
// {

void RectBoundingBox::render(float cameraX, float cameraY)
{
    if (!this->rect) return;

    Rectangle tmp(this->rect->x - cameraX,
                  this->rect->y - cameraY,
                  this->rect->w,
                  this->rect->h);

    // Random blinking colors up this thing!
    Color color;
    color.randomize();

    Graphics::drawRectangle(tmp, color);
}
void RectBoundingBox::copy(RectBoundingBox* other)
{
    // // Seriously, if you really want to copy a thing
    // // to itself, you MUST be sleepy.
    // //
    // // Just like I was when I wrote this code.
    // if (!(this->externalRect))
    //     return;

    this->rect->x = other->rect->x;
    this->rect->y = other->rect->y;
    this->rect->w = other->rect->w;
    this->rect->h = other->rect->h;

    this->top       = other->top;
    this->bottom    = other->bottom;
    this->leftmost  = other->leftmost;
    this->rightmost = other->rightmost;
}
void RectBoundingBox::update()
{
    this->top    = this->rect->y;
    this->bottom = this->rect->y + this->rect->h;

    this->leftmost  = this->rect->x;
    this->rightmost = this->rect->x + this->rect->w;
}
