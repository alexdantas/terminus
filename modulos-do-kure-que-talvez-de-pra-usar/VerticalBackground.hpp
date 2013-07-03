#ifndef VERTICAL_BACKGROUND_H_DEFINED
#define VERTICAL_BACKGROUND_H_DEFINED

#include <vector>
#include <string>
#include "Sprite.hpp"

struct VBg
{
    Sprite* sprite;
    float   x;
    float   y;
    int     depth;

    VBg(std::string filename, float x, float y, int depth)
    {
        this->sprite = new Sprite(filename);
        this->x = x;
        this->y = y;
        this->depth = depth;
    }
    ~VBg()
    {
        if(this->sprite) delete this->sprite;
    }
};

// Creates a scrollable background.
class VerticalBackground
{
public:
    VerticalBackground(std::string filename, int depth=0);
    virtual ~VerticalBackground();

    void addParallax(std::string filename, int depth=0);
    void scrollUp(int scroll);
    void scrollDown(int scroll);
    void show(float cameraX, float cameraY);

private:
    VBg* bg;
};

#endif /* VERTICAL_BACKGROUND_H_DEFINED */
