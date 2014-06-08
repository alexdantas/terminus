// TODO need to finish implementing all these methods!

#ifndef GRAPHICS_H_DEFINED
#define GRAPHICS_H_DEFINED

#include "SDL.hpp"
#include "Color.hpp"
#include "Shapes.hpp"

/// Prints things on the screen (points, lines, rectangles...).
///
/// This is a huge wrapper on SDL's (and SDL_gfx/SDL_image's) functions.
///
/// They're used to print several things - like lines, rectangles, triangles...
///
/// @note It interfaces a lot with the shapes described on *Shapes.hpp*.
/// @see Shapes.hpp
///
/// ## How to use:
///
///     // See Shapes.hpp to see more
///     Point p(2, 3);
///
///     Graphics::drawPixel(p);
///     Graphics::drawLine(p, Point(10, 14);
///
///     Graphics::drawTriangle(Point(10, 10),
///                            Point(5, 20),
///                            Point(15, 20);
///
class Graphics
{
public:
    /// Initializes graphics mode on SDL *screen*.
    //
    //  @note The *screen* is the one at class *SDL*.
    //  @note If this is not called at all, there will be no
    //        drawings available!
    static void init(SDL_Surface* Screen);

    /// Puts a pixel on *a* with *color*.
    static void drawPixel(Point a, Color color=Color(255, 0, 255));

    /// Draws a line with *color* from *a* to *b*, optionally with *thickness*.
    static void drawLine(Point a, Point b, Color color, int thickness);

    /// Draws a triangle with vertices *a*, *b* and *c* and *color*.
    static void drawTriangle(Point a, Point b, Point c, Color color=Color(255, 0, 255));

    /// Draws an ellipse with center *center* and x/y *radius*.
    ///
    /// ## For example
    /// A circle would be *Graphics::drawEllipse(Point(10, 10), Point(20, 20));*
    static void drawEllipse(Point center, Point radius, Color color=Color(255, 0, 255));

    /// Draws a line from *a* to *b* with *spacing* (in pixels).
    static void drawSpacedLine(Point a, Point b, int spacing, Color color=Color(255, 0, 255));

    /// Draws a rectangle based on *rect*'s dimentions.
    static void drawRectangle(Rectangle rect, Color color=Color(255, 0, 255));

    // /// Draws a circle onscreen.
    // static void drawRectangle(int x, int y, int r, Color color);
    // static void drawCircle(Circle  circ  Color color=Color(255, 0, 255));
    // static void drawCircle(Circle* circ, Color color=Color(255, 0, 255));

private:

    /// The SDL's internal screen data structure.
    static SDL_Surface* screen;
};

#endif /* GRAPHICS_H_DEFINED */
