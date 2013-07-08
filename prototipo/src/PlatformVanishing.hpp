#ifndef PLATFORMVANISHING_H_DEFINED
#define PLATFORMVANISHING_H_DEFINED

#include "Platform.hpp"
#include "TimerCounter.hpp"

/// A special kind of platform that disappears with time.
class PlatformVanishing: public Platform
{
public:
    /// Creates a platform that disappears within a *delay*.
    ///
    /// @note Don't worry, it will only start to vanish when
    ///       you call *vanish()*.
    PlatformVanishing(Sprite* sprite, float x, float y, int w, int h, int delay);

    virtual ~PlatformVanishing();

    /// Tells the platform to start vanishing.
    ///
    /// After the *delay* it will disappear.
    void vanish();

    /// Updates platform's state.
    ///
    /// @note Must be called every frame!
    void update(float dt);

    /// Tells if this platform is currently disappearing.
    bool isVanishing();

private:
    TimerCounter* timer; ///< Counts when to actually disappear.
    bool willVanish;     ///< Tells us if it will vanish.
    bool vanishing;      ///< If we're currently vanishing.
};

#endif //PLATFORMVANISHING_H_DEFINED

