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

    Fade(FadeType type, unsigned int speed);
    virtual ~Fade();

    void update(uint32_t dt);
    void render();

private:
    FadeType type;

    unsigned int speed;

    SDL_Surface* surface;

    Color color;
};

#endif //FADE_H_DEFINED

