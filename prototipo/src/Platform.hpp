#ifndef PLATFORM_H_DEFINED
#define PLATFORM_H_DEFINED

#include "GameObject.hpp"
#include "Sprite.hpp"

///
class Platform: public GameObject
{
public:
    Platform(Sprite* sprite, float x, float y, int w, int h);
    virtual ~Platform();

    void render(float cameraX, float cameraY);
    void update(uint32_t dt);

private:
    Sprite* sprite;
};

#endif //PLATFORM_H_DEFINED

