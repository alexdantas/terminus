#ifndef THORN_H_DEFINED
#define THORN_H_DEFINED

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Shapes.hpp"
#include "Config.hpp"
#include "Graphics.hpp"

class Thorn : public GameObject
{
public:
    Thorn(std::string file, int x, int y, int w, int h);
    virtual ~Thorn();

    void update(float dt);
    void render(float cameraX, float cameraY);

    void setPositionCollision(int y);
    void setPositionSprite(int y);
    void setPosition(float x, float y);
    void addPosition(float x, float y);

private:
    Sprite* sprite;
    Rectangle* desiredPosition;
};

#endif //THORN_H_DEFINED

