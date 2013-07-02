#ifndef CLOUD_H_DEFINED
#define CLOUD_H_DEFINED

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Shapes.hpp"

/// Defines a single cloud to use on the background.
///
/// It moves by itself.
///
class Cloud: public GameObject
{
public:
    Cloud(float x, float y, float speed, Sprite* sprite, unsigned int id=0);
    virtual ~Cloud();

    void update(uint32_t dt);
    void render(float cameraX, float cameraY);

    void setPosition(Point p);
    void setSpeed(float speed);

    bool isVisible();
    void setVisible(bool set);

private:
    unsigned int id;

    float speedX;
    Sprite* sprite;

    bool visible;
};

#endif //CLOUD_H_DEFINED

