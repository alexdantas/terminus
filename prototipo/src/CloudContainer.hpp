#ifndef CLOUDCONTAINER_H_DEFINED
#define CLOUDCONTAINER_H_DEFINED

#include <vector>
#include "Cloud.hpp"
#include "Sprite.hpp"
#include "Shapes.hpp"
#include "TimerCounter.hpp"

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
    /// These are the different types of clouds we have.
    enum CloudType
    {
        CLOUD_1, CLOUD_2, CLOUD_3,
        CLOUD_MAX // This arbitrary value exists so the vector can be
                  // safely resized to contain all possible clouds.
    };

    /// Creates a cloud container that holds *maxAmmount* of clouds
    /// that will be inserted inside *areaLimit*.
    ///
    /// @note *blackClouds* is a little hack to allow us to have two
    ///       sets of cloud sprites - black and white.
    ///       Don't change this, it's ugly as fuck.
    CloudContainer(unsigned int maxAmmount, Rectangle areaLimit, bool blackClouds=true);

    virtual ~CloudContainer();

    ///
    void update(float dt);

    ///
    void render(float cameraX=0, float cameraY=0);

    ///
    void addAt(Point p, float speed);

    /// Adds a cloud at random inside the delimited area.
    void addAtRandom();

    /// Adds all possible clouds (based on the *maxAmmount*).
    void addAll();

    ///
    void removeAt(unsigned int index);

    ///
    void limitArea(Rectangle a);

private:
    /// Maximum size of clouds allowed.
    unsigned int maxAmmount;

    /// Current ammount of clouds.
    unsigned int currentAmmount;

    std::vector<Cloud*> freeClouds;
    std::vector<Cloud*> usedClouds;

    std::vector<Sprite*> sprites;

    /// Contains indexes to active objects.
    std::vector<int> indirection;

    Rectangle areaLimit;

    TimerCounter* timer;
};

#endif //CLOUDCONTAINER_H_DEFINED

