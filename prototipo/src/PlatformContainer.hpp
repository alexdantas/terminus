#ifndef PLATFORMCONTAINER_H_DEFINED
#define PLATFORMCONTAINER_H_DEFINED

#include <vector>
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
    // All types of platforms.
    //
    // @note Some are NOT implemented at all!
    enum PlatformType
    {
        GROUND=0, CLOUD, VANISHING, MOVABLE,
        PLATFORM_MAX // This arbitrary value exists so the vector
                     // can be safely resized to contain all
                     // possible platforms.
    };

    /// Creates a platform container that holds *maxAmmount* of
    /// platforms that will be inserted inside *areaLimit*.
    PlatformContainer(unsigned int maxAmmount, Rectangle areaLimit);

    virtual ~PlatformContainer();

    ///
    void add(Point p, PlatformType type);
    void addBetween(Point a, Point b, PlatformType type=PLATFORM_MAX);

    // /// Adds a platform at random inside the delimited area.
    // void addAtRandom();

    /// Adds all possible platforms (based on the *maxAmmount*).
    void addAll();

    // ///
    // void removeAt(unsigned int index);

    ///
    void limitArea(Rectangle a);

    void render(float cameraX, float cameraY);
    void update(uint32_t dt);

    bool collidesWith(GameObject* other);

private:
    /// Maximum size of platforms allowed.
    unsigned int maxAmmount;

    /// Current ammount of platforms.
    unsigned int currentAmmount;

    std::vector<Platform*> usedPlatforms;

    std::vector<Sprite*> sprites;

    /// Contains indexes to active objects.
    std::vector<int> indirection;

    Rectangle areaLimit;
};

#endif //PLATFORMCONTAINER_H_DEFINED

