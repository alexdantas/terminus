#include "Background.hpp"

Background::Background(std::string filename, int depth)
{
    this->bg.push_back(new Bg(filename, 0, 0, depth));
}
Background::~Background()
{
    // FIND OUT THE PROBLEM HERE
    int size = this->bg.size();
    // for (int i = 0; i < size; i++)
    //     if (this->bg[i])
    //         delete this->bg[i];
}
void Background::addParallax(std::string filename, int depth)
{
    this->bg.push_back(new Bg(filename, 0, 0, depth));
}
void Background::scrollRight(int scroll)
{
   int size = this->bg.size();
    for (int i = 0; i < size; i++)
    {
        this->bg[i]->x += (scroll * scroll/(1 + this->bg[i]->depth));

        if ((this->bg[i]->x) >= (this->bg[i]->sprite->getWidth()))
            this->bg[i]->x = 0;
    }
}
void Background::scrollLeft(int scroll)
{
   int size = this->bg.size();
    for (int i = 0; i < size; i++)
    {
        this->bg[i]->x -= (scroll * scroll/(1 + this->bg[i]->depth));

        if ((this->bg[i]->x) <= (-1 * this->bg[i]->sprite->getWidth()))
            this->bg[i]->x = 0;
    }
}
void Background::show(float camera, float cameraY)
{
    int size = this->bg.size();
    for (int i = 0; i < size; i++)
    {
        this->bg[i]->sprite->render(this->bg[i]->x - this->bg[i]->sprite->getWidth());
        this->bg[i]->sprite->render(this->bg[i]->x);
        this->bg[i]->sprite->render(this->bg[i]->x + this->bg[i]->sprite->getWidth());
    }
}

