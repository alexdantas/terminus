#ifndef BADGUYVENUS_H_DEFINED
#define BADGUYVENUS_H_DEFINED

#include "BadGuy.hpp"
#include "TimerCounter.hpp"
#include "Sprite.hpp"

/**
* Venus is a bad godness
*
*
* In Venus class, the only changes are:
* - Sprite sheet (duh)
* - The way she don't move
* - Super heart beam
*
* More details in Bad Guy container class ;)
*/
class BadGuyVenus : public BadGuy
{
public:
    BadGuyVenus(float x, float y, int w, int h, int hp, float acceleration);
    virtual ~BadGuyVenus();
    void update(float dt); //Thw way Venus don't move
    void updateAttack(float dt);

    /// Makes the Venus step into a movable platform *platform*.
    ///
    /// This is necessary if we want the Venus to move along with
    /// a movable platform.
    /// Whenever the platform moves, the Venus will move with it.
    ///
    /// @note Please send NULL if Venus not standing on
    ///       any movable platform.
    void stepIntoMovablePlatform(PlatformMovable* platform);
private:
    TimerCounter *timer;

    PlatformMovable* movablePlatform;

};

#endif //BADGUYVENUS_H_DEFINED

