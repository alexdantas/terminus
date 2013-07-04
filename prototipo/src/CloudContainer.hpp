#ifndef CLOUDCONTAINER_H_DEFINED
#define CLOUDCONTAINER_H_DEFINED

#include <vector>
#include "Cloud.hpp"
#include "Sprite.hpp"
#include "Shapes.hpp"

/// Handles cloud's inclusions and deletions on the game.
///
/// A set of clouds to be used on the background.
///
/// ## For developers
/// It internally uses a pool of pre-allocated objects, instead of
/// dynamically using *new* and *delete*.
///
class CloudContainer
{
public:
    CloudContainer(unsigned int maxAmmount, Rectangle areaLimit);
    virtual ~CloudContainer();

    void update(uint32_t dt);
    void render(float cameraX, float cameraY);

    void addAt(Point p);
    void addAtRandom();
    void removeAt(unsigned int index);

    void limitArea(Rectangle a);

private:
    unsigned int maxAmmount;
    unsigned int currentAmmount;

    std::vector<Cloud*> freeClouds;
    std::vector<Cloud*> usedClouds;
    Sprite* sprite;

    /// Contains indexes to active objects.
    std::vector<int> indirection;

    bool hasLimits;
    Rectangle areaLimit;
};

#endif //CLOUDCONTAINER_H_DEFINED

