#include "Fade.hpp"
#include "Window.hpp"
#include "Graphics.hpp"

Fade::Fade(FadeType type, float speed, Color color):
    type(type),
    speed(speed),
    surface(NULL),
    done(false),
    started(false)
{
    this->surface = SDL::newSurface(Window::width, Window::height);

    if (this->type == FADE_IN)
        this->color = Color(color.r(),
                            color.g(),
                            color.b(), SDL_ALPHA_OPAQUE);

    else if (this->type == FADE_OUT)
        this->color = Color(color.r(),
                            color.g(),
                            color.b(), SDL_ALPHA_TRANSPARENT);

    else
    { /* FUCK */ }
}
Fade::~Fade()
{
    if (this->surface)
        SDL::deleteSurface(this->surface);
}
void Fade::start()
{
    this->done    = false;
    this->started = true;
}
void Fade::stop()
{
    this->done    = true;
    this->started = false;
}
void Fade::update(float dt)
{
    if (!(this->started) ||
        (this->done))
        return;

    // TODO: Create other systems.
    // current: linear
    // possible: exponential, logarythimic, whatever

    int newAlpha; // Changed alpha value

    if (this->type == FADE_IN)
    {
        newAlpha = (this->color.a() - (this->speed * dt));

        if (newAlpha <= SDL_ALPHA_TRANSPARENT)
        {
            this->done = true;
            return;
        }
    }

    else if (this->type == FADE_OUT)
    {
        newAlpha = (this->color.a() + (this->speed * dt));

        if (newAlpha >= SDL_ALPHA_OPAQUE)
        {
            this->done = true;
            return;
        }
    }

    else
    { /* FUCK */ }

    this->color = Color(this->color.r(),
                        this->color.g(),
                        this->color.b(), (uint8_t)newAlpha);

    SDL_SetAlpha(this->surface, SDL_SRCALPHA, this->color.a());
}
void Fade::render()
{
    // If we've not started, fade in will show as black and fade
    // out will not show at all.
    //
    // If we've finished, fade in will not show at all and fade
    // out will be shown as black.

    if (this->type == FADE_OUT)
        if (!(this->started))
            return;

    if (this->type == FADE_IN)
        if (this->done)
            return;

    // Actually drawing the color
    SDL_Rect tmp;
    tmp.x = 0;
    tmp.y = 0;
    tmp.w = Window::width;
    tmp.h = Window::height;

    SDL_Rect tmp2;
    tmp2.x = 0;
    tmp2.y = 0;

    SDL_FillRect(this->surface, &tmp, SDL::convertColorFormat(this->color));

    SDL::renderSurface(this->surface, NULL, &tmp2);
}
bool Fade::isDone()
{
    return (this->done);
}

