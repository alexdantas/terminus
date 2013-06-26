#ifndef PLATFORMMANAGER_H_DEFINED
#define PLATFORMMANAGER_H_DEFINED

#include <vector>
#include "Sprite.hpp"
#include "Platform.hpp"
#include "Shapes.hpp"

///
class PlatformManager
{
public:
    enum PlatformType { GROUND=0, CLOUD, VANISHING, TYPE_MAX };

    PlatformManager();
    virtual ~PlatformManager();

    void add(Point x, PlatformType type=GROUND);
    void addBetween(Point a, Point b, PlatformType type=TYPE_MAX);

    void render(float cameraX, float cameraY);
    void update(uint32_t dt);

    bool collidesWith(GameObject* other);

private:
    std::vector<Platform*> container;
    std::vector<Sprite*> sprites;
    bool empty;
};

#endif //PLATFORMMANAGER_H_DEFINED

