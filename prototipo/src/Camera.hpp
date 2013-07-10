#ifndef CAMERA_H_DEFINED
#define CAMERA_H_DEFINED

/// Decides what things are shown on the screen.
///
/// This camera is VERY LIMITED.
/// It currently only works on games that scroll up (like this one :D).
///
/// Here's the logic:
///
/// ## Object rendering relative to the camera
///
/// * This camera has x and y that _must_ be passed around to all
///   objects. This way, all of them are going to be rendered
///   _relative_ to the camera instead of _absolutely_ on the screen.
///
/// * Then, at each frame you must call *centerOn()* to make the
///   camera center it's *x* and *y* on any object.
///
/// If you do the two steps above, all objects of the game will be
/// shown OK, regardless of the camera position.
///
/// ## Camera movement limiting
///
/// You can limit where the camera can go. This way the game
/// won't be infinite.
///
/// You can call *setVerticalLimit()* to prevent the camera from
/// getting out of a delimited vertical area;
/// and you can call *lockXAxis()* to make the camera only move
/// on the Y axis.
///
class Camera
{
public:
    /// Creates a camera at *x*, *y*, *w* and *h* with *scrollSpeed*.
    Camera(float x, float y, int w, int h);

    float getX();
    float getY();
    int   getWidth();
    int   getHeight();
    void  setX(float x);
    void  setY(float y);

    /// Centers the camera on *x* and *y*.
    void centerOn(int x, int y);

    /// Prevents the camera from moving on the x axis.
    void lockXAxis(bool option=true);

    /// Sets a vertical limit that the camera never scrolls over.
    void setVerticalLimit(int top, int bottom);

private:
    float x; ///< Current camera position on the x-axis.
    float y; ///< Current camera position on the y-axis.
    int w;   ///< Current camera width.
    int h;   ///< Current camera height.

    bool lockedX; ///< Is it locked on the X axis?
    bool lockedY; ///< Is it locked on the Y axis?

    bool hasVerticalLimit; ///< Does it have a vertical limit?
    int  topLimitY;        ///< The top vertical limit.
    int  bottomLimitY;     ///< The bottom vertical limit.
};

#endif /* CAMERA_H_DEFINED */

