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
class Point
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
/// @note Pretty much EVERYTHING's public. DO NOT mess with it
///       directly -- use my predefined functions! They refresh
///       the internal values nicely.
///       I've made everything public so we could access it
///       on a consistent way (Rectangle.x, for example).
///
/// @note NEVER try to modify *x/y/w/h* directly! Always do it
///       through the *add()*/*set()* methods!
///       That's because this class needs to refresh internal
///       points and stuff.
class Rectangle
{
public:
    float x; ///< X of the top-left point
    float y; ///< Y of the top-left point
    int w; ///< Width
    int h; ///< Height

    float top;       ///< An alias for Y
    float bottom;    ///< An alias for Y + H
    float leftmost;  ///< An alias for X
    float rightmost; ///< An alias for X + W

    // Special points, pretty much self-explanatory
    Point center;
    Point leftCenter;
    Point rightCenter;
    Point topLeft;
    Point topRight;
    Point topCenter;
    Point bottomLeft;
    Point bottomRight;
    Point bottomCenter;

    /// Creates a sweet rectangle with *x* and *y* coordinates and
    /// *w* and *y* dimensions.
    Rectangle(float x, float y, int w, int h);

    /// Creates a rectangle from two opposite corners.
    ///
    /// TODO Implement!
    Rectangle(Point topLeft, Point bottomRight);

    Rectangle();

    /// Copies *other*'s internal values to this.
    void copy(Rectangle* other);

    /// Adds to the current x, making necessary internal calculations.
    ///
    /// Of course, if you want to subtract, send a negative parameter.
    void addX(float x);

    /// Adds to the current x, making necessary internal calculations.
    ///
    /// Of course, if you want to subtract, send a negative parameter.
    void addY(float y);

    /// Sets the current x, making necessary internal calculations.
    void setX(float x);

    /// Sets the current y, making necessary internal calculations.
    void setY(float y);

    /// Stretches the rectangle by *step*.
    ///
    /// If you want to increase it's size by two, send 2.
    /// If you want to shrink it by two, send 0.2.
    void stretch(float scale);
    void stretch(float scaleX, float scaleY);

    /// Refreshes all internal values.
    ///
    /// This gets called all the time internally, don't worry
    /// about it.
    ///
    /// God, I'm sleepy.
    void update();

    /// Tells if this rectangle overlaps the other in any way.
    bool overlaps(Rectangle other);

    /// Tells if this rectangle overlaps the other in any way.
    bool overlaps(Rectangle* other);

    /// Places this rectangle immediately above *other*.
    ///
    /// @note It only considers the y axis, x is left unchanged.
    ///
    /// ## For developers
    ///
    /// I've only created this method to make possible for the player
    /// to stay above a platform.
    /// Say if it's collision Rectangle passes through the platform,
    /// we place it above it with this method.
    void placeOnTop(Rectangle* other);
};

/// A circle, duh.
///
class Circle
{
public:
    float x;
    float y;
    int r; /// Radius

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
