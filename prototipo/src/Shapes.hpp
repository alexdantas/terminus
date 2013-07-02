/// @file Shapes.hpp
///
///  This file defines a set of basic shapes that can be used
///  for... pretty much whatever.
///  They're used extensively inside my library because they
///  have some nice features.
///
///  For instance, storing important points of the shape
///  (like top-left, center points) to avoid unnecessary
///  calculations.

#ifndef SHAPES_H_DEFINED
#define SHAPES_H_DEFINED

/// A point - how simpler could it be?
///
struct Point
{
public:
    float x;
    float y;

    Point();
    Point(float x, float y);
};

/// A rectangle with special properties.
///
/// It automatically stores all important points of a rectangle
/// (topLeft, topRight, center...), making everything easier.
///
/// @note NEVER try to modify *x/y/w/h* directly! Always do it
///       through the *add()*/*set()* methods!
///       That's because this class needs to refresh internal
///       points and stuff.
struct Rectangle
{
public:
    float x; ///< X of the top-left point
    float y; ///< Y of the top-left point
    unsigned int w; ///< Width
    unsigned int h; ///< Height

    float top;       ///< An alias for Y
    float bottom;    ///< An alias for Y + H
    float leftmost;  ///< An alias for X
    float rightmost; ///< An alias for X + W

    // Special points
    Point center;      ///< Center point.
    Point topLeft;
    Point topRight;
    Point bottomLeft;
    Point bottomRight;

    /// Creates a sweet rectangle with *x* and *y* coordinates and
    /// *w* and *y* dimensions.
    Rectangle(float x, float y, int w, int h);

    /// Adds to the current x, making necessary internal calculations.
    void addX(float x);

    /// Adds to the current x, making necessary internal calculations.
    void addY(float y);

    /// Sets the current x, making necessary internal calculations.
    void setX(float x);

    /// Sets the current y, making necessary internal calculations.
    void setY(float y);

    /// Refreshes all internal values.
    ///
    /// This gets called all the time internally, don't worry
    /// about it.
    ///
    /// God, I'm sleepy.
    void recalculate();
};

/// A circle, duh.
///
struct Circle
{
    float x;
    float y;
    unsigned int r; /// Radius

    float centerX;
    float centerY;

    /// Creates a nice circle with *x* and *y* coordinates and *r*
    /// radius.
    Circle(float x, float y, int r);

    /// Adds to the current x.
    /// Although this seems useless, you must call this instead of
    /// modifying x directly because it refreshes internal points.
    void addX(float x);

    /// Adds to the current y.
    /// Although this seems useless, you must call this instead of
    /// modifying y directly because it refreshes internal points.
    void addY(float y);
};

#endif /* SHAPES_H_DEFINED */
