#ifndef PLATFORMCONTAINER_H_DEFINED
#define PLATFORMCONTAINER_H_DEFINED

#include <vector>
#include <list>
#include "Platform.hpp"
#include "Sprite.hpp"
#include "Shapes.hpp"
#include "GameObject.hpp"

/// Contains all platforms for the game.
///
/// A set of platforms to be used.
///
class PlatformContainer
{
public:
    /// Creates a platform container that holds *maxAmmount* of
    /// platforms that will be inserted inside *areaLimit*.
    PlatformContainer(unsigned int maxAmmount, Rectangle areaLimit);

    virtual ~PlatformContainer();

    /// Adds a platform with *type* on point *p*.
    void add(Point p, Platform::PlatformType type);

    /// Adds a platform between points *a* and *b* with *type*.
    ///
    /// @note If no type is specified, will add a random one.
    ///
    /// ## For developers
    ///
    /// *a* and *b* are the opposite vertices of a rectangle, thus
    /// delimiting an area on which I can insert a platform.
    ///
    /// Like this:
    ///
    ///     a---------.
    ///     |         | <- where I'll add platforms
    ///     .---------b
    ///
    void addBetween(Point a, Point b, Platform::PlatformType type=Platform::MAX);

    /// Limits the platform-generation area to *a*.
    void limitArea(Rectangle a);

    /// Shows all the platforms on the screen (delimited by
    /// *cameraX* and *cameraY*).
    void render(float cameraX, float cameraY);

    /// Updates all platforms.
    void update(float dt);

    /// Tells if any platform collides with *other*.
//    bool collidesWith(GameObject* other);

    /// Returns the uppermost platform on the map.
    Platform* getTopPlatform();

    /// Deletes the last platform (bottom platform
    /// on the game area).
    void deleteLast();

    /// Tells if we've already spawned the whole set of platforms.
    bool isFull();

    // TODO HACK WHATEVER
    //
    // This was made public because the GameState needs to check on
    // it to compare with the player on any collisions.
    //
    // I need to find a better way to do so.

    /// The whole raw list of platforms.
    std::list<Platform*> platforms;

private:
    /// Maximum size of platforms allowed.
    unsigned int maxAmmount;

    /// Current ammount of platforms.
    unsigned int currentAmmount;

    /// All possible sprites of the different platform types.
    std::vector<Sprite*> sprites;

    /// Contains indexes to active objects.
    std::vector<int> indirection;

    /// The area on which the platforms can be added.
    ///
    /// Will not insert outside of this, don't worry.
    Rectangle areaLimit;
};

#endif //PLATFORMCONTAINER_H_DEFINED

