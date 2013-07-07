#ifndef PLATFORM_H_DEFINED
#define PLATFORM_H_DEFINED

#include "GameObject.hpp"
#include "Sprite.hpp"

///
class Platform: public GameObject
{
public:
    Platform(Sprite* sprite, float x, float y, int w, int h, unsigned int id=0);
    virtual ~Platform();

    void render(float cameraX, float cameraY);
    void update(float dt);

    void setPosition(Point p);

    bool isVisible();
    void setVisible(bool option);

private:
    unsigned int id;

    Sprite* sprite;

    bool visible;
};

#endif //PLATFORM_H_DEFINED

