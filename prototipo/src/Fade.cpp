#include "Fade.hpp"
#include "Window.hpp"
#include "Graphics.hpp"

Fade::Fade(FadeType type, unsigned int speed):
    type(type),
    speed(speed),
    surface(NULL)
{
    this->surface = SDL::newSurface(Window::width, Window::height);

    if (this->type == FADE_IN)
        this->color = Color(0, 0, 0, SDL_ALPHA_OPAQUE);

    else if (this->type == FADE_OUT)
        this->color = Color(0, 0, 0, SDL_ALPHA_TRANSPARENT);

    else
    { /* FUCK */ }
}
Fade::~Fade()
{
    if (this->surface)
        SDL::deleteSurface(this->surface);
}
void Fade::update(uint32_t dt)
{
    // TODO: Create other systems.
    // current: linear
    // possible: exponential, logarythimic, whatever

    uint8_t newAlpha; // Changed alpha value

    if (this->type == FADE_IN)
    {
        if (this->color.a() != SDL_ALPHA_TRANSPARENT)
            newAlpha = (this->color.a() - this->speed);
    }

    else if (this->type == FADE_OUT)
    {
        if (this->color.a() != SDL_ALPHA_OPAQUE)
            newAlpha = (this->color.a() + this->speed);
    }

    else
    { /* FUCK */ }

    this->color = Color(0, 0, 0, newAlpha);
    SDL_SetAlpha(this->surface, SDL_SRCALPHA, this->color.a());
}
void Fade::render()
{
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

