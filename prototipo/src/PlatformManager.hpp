#ifndef PLATFORMMANAGER_H_DEFINED
#define PLATFORMMANAGER_H_DEFINED

#include <vector>
#include "Sprite.hpp"
#include "Platform.hpp"
#include "Shapes.hpp"
#include "PlatformContainer.hpp"

/// Handles the platforms' inclusions and deletions on the game.
///
/// Has the rules to include them.
///
class PlatformManager
{
public:
    PlatformManager(Rectangle gameArea);
    virtual ~PlatformManager();

    void update(float dt);
    void render(float cameraX, float cameraY);

    /// All the platforms of the game.
    PlatformContainer* container;
private:
};

#endif //PLATFORMMANAGER_H_DEFINED

