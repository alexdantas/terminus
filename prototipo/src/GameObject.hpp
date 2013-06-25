#ifndef GAMEOBJECT_H_DEFINED
#define GAMEOBJECT_H_DEFINED

#include <stdint.h> // uint32_t
#include "Shapes.hpp"
#include "RectBoundingBox.hpp"

///
///
///
/// The collision scheme was based on Rodrigo Monteiro's awesome
/// "Guide to Implementing 2D Platformers":
/// http://higherorderfun.com/blog/2012/05/20/the-guide-to-implementing-2d-platformers/
///
class GameObject
{
public:
    GameObject(float x=0.0, float y=0.0, int w=0, int h=0);
    ~GameObject();

    /// Updates all the object's internal states.
    virtual void update(uint32_t dt) = 0;

    /// Shows object onscreen.
    virtual void render(float cameraX, float cameraY) = 0;

    /// Tells if we've collided with another thing.
    bool collidedWith(GameObject* other);

    /// Tells if we've one-way-collided with other thing.
    ///
    /// This is a special way of collision.
    /// Remember Super Mario? Those platforms on which Mario
    /// could step on _and_ jump through?
    /// They use one-way collision, on the sense that you can
    /// go through them but still they offer collision when
    /// you're coming from above.
    bool oneWayCollidedWith(GameObject* other);

    float getX();
    float getY();
    float getCenterX();
    float getCenterY();
    int   getWidth();
    int   getHeight();

protected:

    /// Object's size and position.
    Rectangle* box;

    /// Collision check box.
    RectBoundingBox* boundingBox;

    /// Last frame's bounding box.
    ///
    /// This is optional. You only have to use it when considering
    /// comples collision resolution systems.
    RectBoundingBox* previousBoundingBox;

};

#endif /* GAMEOBJECT_H_DEFINED */
