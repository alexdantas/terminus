#ifndef PLATFORM_H_DEFINED
#define PLATFORM_H_DEFINED

#include "GameObject.hpp"
#include "Sprite.hpp"

/// A 2D platform.
///
/// Hell yeah.
///
class Platform: public GameObject
{
public:
    // All types of platforms.
    //
    // @note Some are NOT implemented at all!
    enum PlatformType
    {
        GROUND=0, NORMAL, VANISHING, MOVABLE, CLOUD,

        MAX // This arbitrary value exists so that any vector
            // that wished to hold this enum can be safely
            // resized to contain all possible platforms.
    };

    /// Create a platform with *sprite* on *x*, *y*, with *w* and *h*.
    Platform(Sprite* sprite, float x, float y, int w, int h, PlatformType type=GROUND);

    virtual ~Platform();

    /// Show the platform on the screen.
    void render(float cameraX, float cameraY);

    /// Updates platform.
    ///
    /// We don't actually do nothing here (since this is a standing
    /// platform, dammit), but other kinds of platforms may be
    /// able to overload this (like movable platforms).
    virtual void update(float dt);

    void setPosition(Point p);

    /// Tells if the platform is visible.
    bool isVisible();

    /// We'll set it as visible or invisible.
    ///
    /// @note If we set a platform as invisible, it won't appear
    ///       at the game not even for collision-checking!
    void setVisible(bool option);

    /// The type of the platform.
    ///
    /// TODO a redesign of the class hierarchy?
    ///      Ideally one class shouldn't have an explicit thing
    ///      saying what is their kind.
    PlatformType type;

    /// If it's currently (or it ever was) occupied by some entity.
    bool occupied;

protected:
    Sprite* sprite;  ///< Appearance on the screen.
    bool visible;    ///< Is this visible?
};

#endif //PLATFORM_H_DEFINED

