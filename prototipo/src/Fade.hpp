#ifndef FADE_H_DEFINED
#define FADE_H_DEFINED

#include "SDL.hpp"

/// Makes the screen fade in or out.
///
/// Allows smooth transition between screens.
///
class Fade
{
public:
    enum FadeType { FADE_IN, FADE_OUT };

    Fade(FadeType type, float speed);
    virtual ~Fade();

    /// Starts the fading effect.
    void start();
    void stop();

    void update(uint32_t dt);
    void render();

    /// Tells if the fade has finished.
    bool isDone();

private:
    FadeType type;

    float speed;

    SDL_Surface* surface;

    Color color;

    bool done;
    bool started;
};

#endif //FADE_H_DEFINED

