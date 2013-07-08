#ifndef PLATFORMMOVABLE_H_DEFINED
#define PLATFORMMOVABLE_H_DEFINED

#include "Platform.hpp"

/// A special kind of platform that moves around.
///
/// Note that it moves independently of anything else.
/// If you want an object to move along with the platform,
/// you have to create a pointer to this on the object.
///
/// There's an example of this on the *Player* class.
///
/// Inspired on Rodrigo Monteiro's 2D Guide to Platform Games.
///
class PlatformMovable: public Platform
{
public:
    /// YEEEEEAH
    enum FacingDirection { LEFT, RIGHT };

    PlatformMovable(Sprite* sprite, float x, float y, int w, int h, float speed);

    virtual ~PlatformMovable();

    void update(float dt);
    void setHorizontalLimit(int left, int right);
    void setVerticalLimit(int top, int bottom);

    Rectangle previousBox;

private:
    // /// Flag to identify this specific kind of platform.
    // Platform::PlatformType type;

    float speed;

    FacingDirection direction;

    /// Tells if the platform's limited by some distance.
    bool hasHorizontalLimit;

    int rightmostLimitX;
    int leftmostLimitX;

    bool hasVerticalLimit;

    int topLimitY;
    int bottomLimitY;
};

#endif //PLATFORMMOVABLE_H_DEFINED

