#include "Graphics.hpp"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_image.h"

SDL_Surface* Graphics::screen = NULL;

void Graphics::init(SDL_Surface* Screen)
{
    screen = Screen;
}
void Graphics::drawPixel(Point a, Color color)
{
    if (!screen) return;

    pixelRGBA(screen, a.x, a.y, color.r(), color.g(), color.b(), color.a());
}
void Graphics::drawTriangle(Point a, Point b, Point c, Color color)
{
    if (!screen) return;

    trigonRGBA(screen, a.x, a.y, b.x, b.y, c.x, c.y,
               color.r(), color.g(), color.b(), color.a());
}
void Graphics::drawEllipse(Point center, Point radius, Color color)
{
    if (!screen) return;

    ellipseRGBA(screen, center.x, center.y, radius.x, radius.y,
                color.r(), color.g(), color.b(), color.a());
}
void Graphics::drawLine(Point a, Point b, Color color, int thickness)
{
    if (!screen) return;

    thickLineRGBA(screen, a.x, a.y, b.x, b.y, thickness,
                  color.r(), color.g(), color.b(), color.a());
}
void Graphics::drawSpacedLine(Point a, Point b, int spacing, Color color)
{
    // This method SUCKS and it's very BADLY programmed.
    // I've got it from a friend and still haven't been able to
    // decipher it (20/06/2013).
    //
    // If somewhat you can, please e-mail me:
    // Alexandre Dantas <eu@alexdantas.net>

    if (!screen) return;

    int deltax = abs(b.x - a.x); // The difference between the x's
    int deltay = abs(b.y - a.y); // The difference between the y's
    int x = a.x;                // Start x off at the first pixel
    int y = a.y;                // Start y off at the first pixel
    int xinc1 = 0;
    int yinc1 = 0;
    int xinc2 = 0;
    int yinc2 = 0;
    int den = 0;
    int num = 0;
    int numadd = 0;
    int numpixels = 0;

    if (b.x >= a.x) // The x-values are increasing
        xinc1 =  1;
    else            // The x-values are decreasing
        xinc1 = -1;

    if (b.y >= a.y) // The y-values are increasing
        yinc1 =  1;
    else            // The y-values are decreasing
        yinc1 = -1;

    xinc2 = xinc1;
    yinc2 = yinc1;

    if (deltax >= deltay)        // There is at least one x-value for every y-value
    {
        xinc1 = 0;                // Don't change the x when numerator >= denominator
        yinc2 = 0;                // Don't change the y for every iteration
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;        // There are more x-values than y-values
    }
    else                        // There is at least one y-value for every x-value
    {
        xinc2 = 0;                // Don't change the x for every iteration
        yinc1 = 0;                // Don't change the y when numerator >= denominator
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;        // There are more y-values than x-values
    }

    for (int curpixel = 1; curpixel <= numpixels; curpixel++)
    {
        if (spacing == 0)
            Graphics::drawRectangle(Rectangle(x, y, 1, 1), color);

        else if ((curpixel % spacing) >= (spacing / 2))
            Graphics::drawRectangle(Rectangle(x, y, 1, 1), color);

        num += numadd;         // Increase the numerator by the top of the fraction
        if (num >= den)        // Check if numerator >= denominator
        {
            num -= den;         // Calculate the new numerator value
            x += xinc1;         // Change the x as appropriate
            y += yinc1;         // Change the y as appropriate
        }
        x += xinc2;             // Change the x as appropriate
        y += yinc2;             // Change the y as appropriate
    }
}
void Graphics::drawRectangle(Rectangle rect, Color color)
{
    // This is kinda messy, but SDL has it's own Rectangle
    // and Color implementations.

    SDL_Rect tmp;
    tmp.x = rect.x;
    tmp.y = rect.y;
    tmp.w = rect.w;
    tmp.h = rect.h;

    SDL_FillRect(screen, &tmp, SDL::convertColorFormat(color));
}


