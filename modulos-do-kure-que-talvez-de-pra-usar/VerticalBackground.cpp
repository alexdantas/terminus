#include "VerticalBackground.hpp"

VerticalBackground::VerticalBackground(std::string filename, int depth)
{
    this->bg = new VBg(filename, 0,   0, depth);
}
VerticalBackground::~VerticalBackground()
{
    if (this->bg) delete this->bg;
}
void VerticalBackground::scrollUp(int scroll)
{
   int size = this->bg.size();
   for (int i = 0; i < size; i++)
   {
       this->bg[i]->y += (scroll * scroll/(1 + this->bg[i]->depth));

       if ((this->bg[i]->y) >= (this->bg[i]->sprite->getWidth()))
           this->bg[i]->y = 0;
   }
}
void VerticalBackground::scrollDown(int scroll)
{
   int size = this->bg.size();
   for (int i = 0; i < size; i++)
   {
       this->bg[i]->y -= (scroll * scroll/(1 + this->bg[i]->depth));

       if ((this->bg[i]->y) <= (-1 * this->bg[i]->sprite->getWidth()))
           this->bg[i]->y = 0;
   }
}
void VerticalBackground::show(float cameraX, float cameraY)
{
    static float oldCameraX = 0;
    static float oldCameraY = 0;

    int height = this->bg->sprite->getHeight();

    float posFirst  = cameraY - (this->bg->y - height);
    float posMiddle = this->bg->y;
    float posLast   = cameraY - (this->bg->y + height);

    this->bg[i]->sprite->render(0, posFirst);
    this->bg[i]->sprite->render(0, posMiddle);
    this->bg[i]->sprite->render(0, posLast);

    oldCameraX = cameraX;
    oldCameraY = cameraY;
}

