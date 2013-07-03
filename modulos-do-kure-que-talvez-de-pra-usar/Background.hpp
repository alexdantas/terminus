#ifndef BACKGROUND_H_DEFINED
#define BACKGROUND_H_DEFINED

#include <vector>
#include <string>
#include "Sprite.hpp"

struct Bg
{
    Sprite* sprite;
    float   x;
    float   y;
    int     depth;

    Bg(std::string filename, float x, float y, int depth)
    {
        this->sprite = new Sprite(filename);
        this->x = x;
        this->y = y;
        this->depth = depth;
    }
    ~Bg()
    {
        if(this->sprite) delete this->sprite;
    }
};

// Creates a scrollable background.
class Background
{
public:
    Background(std::string filename, int depth=0);
    virtual ~Background();

    void addParallax(std::string filename, int depth=0);
    void scrollRight(int scroll);
    void scrollLeft(int scroll);
    void show(float cameraX, float cameraY);

private:
    std::vector<Bg*> bg;
};

#endif /* BACKGROUND_H_DEFINED */
