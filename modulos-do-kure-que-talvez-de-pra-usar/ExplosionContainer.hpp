#ifndef EXPLOSIONCONTAINER_H_DEFINED
#define EXPLOSIONCONTAINER_H_DEFINED

#include <vector>
#include "Explosion.hpp"
#include "Animation.hpp"
#include "Shapes.hpp"

/// Handles explosion's inclusions and deletions on the game.
///
/// A set of explosions to be used anywhere. Watch out the order on
/// which you render these.
///
/// ## For developers
/// It internally uses a pool of pre-allocated objects, instead of
/// dynamically using *new* and *delete*.
///
class ExplosionContainer
{
public:
    enum ExplosionType { EXPLOSION_1, EXPLOSION_2, EXPLOSION_MAX };

    ExplosionContainer(unsigned int maxAmmount, Rectangle areaLimit);
    virtual ~ExplosionContainer();

    void update(uint32_t dt);
    void render(float cameraX, float cameraY);

    void addAt(Point p);
    void addAtRandom();
    void removeAt(unsigned int index);

    void limitArea(Rectangle a);

private:
    unsigned int maxAmmount;
    unsigned int currentAmmount;

    std::vector<Explosion*> freeExplosions;
    std::vector<Explosion*> usedExplosions;

    std::vector<Animation*> sprites;

    /// Contains indexes to active objects.
    std::vector<int> indirection;

    bool hasLimits;
    Rectangle areaLimit;
};

#endif //EXPLOSIONCONTAINER_H_DEFINED

