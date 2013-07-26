#ifndef BadGuy_H_DEFINED
#define BadGuy_H_DEFINED

#include <vector>
#include "GameObject.hpp"
#include "DamageableObject.hpp"
#include "Animation.hpp"
#include "PlatformMovable.hpp"
#include "Thorn.hpp"

/// Defines the bad guy.
//Actually, pretty similar to the Player's class
//But!!!! Bad guys don't respect rules, even physics rules
class BadGuy: public GameObject,
              public DamageableObject
{
public:
    enum FacingDirection { LEFT, RIGHT };
    enum PossibleAnimation
    {
        STANDING_LEFT = 0,  STANDING_RIGHT,
        DAMAGING_LEFT,  DAMAGING_RIGHT,
        ATTACK_LEFT,    ATTACK_RIGHT,
        DEATH_LEFT,     DEATH_RIGHT,

        ANIMATION_MAX // This arbitrary value exists so the
                      // animation vector can be safely resized
                      // to contain all possible animations
    };

    enum BadGuyType{
        VENUS = 0,
        GRIFFIN,

        MAX /// This arbitrary value exists so that any vector
            // that wished to hold this enum can be safely
            // resized to contain all possible platforms.
    };

    BadGuy(float x, float y, int w, int h, int hp, float acceleration);
    virtual ~BadGuy();

    /// Updates everything and _tries_ to move.
    virtual void update(float dt) = 0;

    /// Shows onscreen.
    void render(float cameraX, float cameraY);

    /// Updates internal animation status.
    void updateAnimation(int dt);

    /// _Actually_ moves the bad guy.
    void commitMovement();

    void setHorizontalLimit(int left, int right);
    void setVerticalLimit(int top, int bottom);


    /// Tells if he's already defeat
    bool isAlive();
    bool Attacking();
    bool isHittable();
    bool died();

    /// Bad guys never wins in the end
    void die();

    void dealDamage();
    void Attacked();

    ///AIN'T NOBODY HAVE TIME FOR OO
    Thorn *beam;

protected:
    float vx; ///< Speed component of the x axis.
    float vy; ///< Speed component of the y axis.
    float ax; ///< Current acceleration on the x axis.
    float ay; ///< Current acceleration on the y axis.

    /// How much speed the player wants to have on the current frame.
    float targetVx;
    float targetVy;

    float acceleration; ///< Walking speed.

    Animation* currentAnimation; /// It's current animation.

    /// All possible animations, ready to be selected.
    std::vector<Animation*> animations;

    FacingDirection facingDirection; /// Current facing direction.

    /// Tells if the bad guy's limited by some distance.
    bool hasHorizontalLimit;
    int rightmostLimitX;
    int leftmostLimitX;

    bool hasVerticalLimit;
    int topLimitY;
    int bottomLimitY;

    bool isAttacking; ///< Is it currently attacking

    bool dead;      ///< Has it died.
    bool damaging;  ///< Is it currently having damage.
    bool visible; ///Is he visible in the scene?

    /// Next position the player wants to be.
    ///
    /// At each frame the player updates it's position on this
    /// *Rectangle*. Then, the *GameState* is responsible for
    /// allowing this or not.
    ///
    /// It might now allow it because of collisions. Then the
    /// player will stay at it's current position.
    ///
    Rectangle* desiredPosition;

    BadGuyType type;

};

#endif //BadGuy_H_DEFINED

