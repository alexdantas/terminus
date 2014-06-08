#ifndef PLATFORMMANAGER_H_DEFINED
#define PLATFORMMANAGER_H_DEFINED

#include <vector>
#include "Sprite.hpp"
#include "Shapes.hpp"
#include "PlatformContainer.hpp"

/// Handles the platforms' inclusions and deletions on the game.
///
/// Has the rules to include them.
///
/// ## For developers
///
/// This baby is very game-specific.
/// On this game, it creates platforms on a restricted *gameArea*,
/// from down up with at most a Y distance of *maxHeight* between
/// each.
///
class PlatformManager
{
public:
    /// Will create platforms inside *gameArea* and with at
    /// most *maxHeight* Y distance between them.
    PlatformManager(Rectangle gameArea, float maxHeight);
    virtual ~PlatformManager();

    /// Sets the frequency of platform-adding.
    ///
    /// @note The lower it is, more crowded platforms are.
    void setFrequency(int frequency);

    /// Updates all platforms and platform handling scheme.
    void update(float dt);

    /// Renders all platforms.
    void render(float cameraX, float cameraY);

    /// All the platforms of the game.
    ///
    /// TODO HACK BUG WHATEVER
    ///
    /// This thing is public because on GameStateGame I need
    /// to check collisions on all of them and the player.
    /// Is there some "cleaner" way of doing this?
    PlatformContainer* container;

    // TMP MTPMT PMTP PM
    void add();

    void addAll();

private:
    /// The maximum allowed size on which the platforms are generated.
    Rectangle gameArea;

    /// Maximum distance on the Y axis between platforms.
    float maxHeight;

    float minHeight;

    /// References the uppermost platform on the game area.
    Platform* topPlatform;

    /// Holds the camera Y to generate platforms.
    float currentCameraY;
};

#endif //PLATFORMMANAGER_H_DEFINED

