#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED

#include <vector>
#include "GameObject.hpp"
#include "DamageableObject.hpp"
#include "Animation.hpp"
#include "PlatformMovable.hpp"

/// Defines the player.
class Player: public GameObject,
              public DamageableObject
{
public:
    enum FacingDirection { LEFT, RIGHT };
    enum PossibleAnimation
    {
        JUMPING_LEFT=0, JUMPING_RIGHT,
        FALLING_LEFT,   FALLING_RIGHT,
        STANDING_LEFT,  STANDING_RIGHT,
        RUNNING_LEFT,   RUNNING_RIGHT,
        DASHING_LEFT,   DASHING_RIGHT,
        DAMAGING_LEFT,  DAMAGING_RIGHT,
        DEATH_LEFT,     DEATH_RIGHT,

        ANIMATION_MAX // This arbitrary value exists so the
                      // animation vector can be safely resized
                      // to contain all possible animations
    };

    Player(float x, float y, int w, int h, int hp, float acceleration);
    virtual ~Player();

    /// Updates everything and _tries_ to move.
    ///
    /// @note Must call *commitMovement()* to allow it to move.
    ///
    /// @see commitMovement()
    void update(float dt);

    /// Shows onscreen.
    void render(float cameraX, float cameraY);

    /// Respond to any input commands.
    void updateInput();

    /// Updates internal animation status.
    void updateAnimation();

    /// _Actually_ moves the player.
    void commitMovement();

    void setHorizontalLimit(int left, int right);
    void setVerticalLimit(int top, int bottom);

    /// Makes the player jump or stops it in mid-air.
    ///
    /// @note If you send *jump(false)* it will stop the
    ///       player completely (on the y axis).
    void jump(bool willJump=true);

    /// Forces the player to suffer gravity.
    void fall();

    /// Toggles _cheat_ fly mode, that allows the player to freely
    /// fly around there.
    void toggleFlyMode();

    /// Makes the player dash, possibly giving damage to others.
    void dash();

    /// Tells if the player's dead.
    bool isHittable();
    bool Dashing();
    bool isFalling();
    bool Winned();

    /// Makes the player die.
    void die();
    bool died();

    void dealDamage();

    /// Makes the player step into a movable platform *platform*.
    ///
    /// This is necessary if we want the player to move along with
    /// a movable platform.
    /// Whenever the platform moves, the player will move with it.
    ///
    /// @note Please send NULL if the player's not standing on
    ///       any movable platform.
    void stepIntoMovablePlatform(PlatformMovable* platform);

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

private:
    float vx; ///< Speed component of the x axis.
    float vy; ///< Speed component of the y axis.
    float ax; ///< Current acceleration on the x axis.
    float ay; ///< Current acceleration on the y axis.

    /// How much speed the player wants to have on the current frame.
    float targetVx;

    float acceleration; ///< Walking speed.

    /// How much of the speed we consider the player stopped.
    float stoppedThreshold;

    Animation* currentAnimation; /// It's current animation.

    /// All possible animations, ready to be selected.
    std::vector<Animation*> animations;

    FacingDirection facingDirection; /// Current facing direction.

    /// Tells if the player's limited by some distance.
    bool hasHorizontalLimit;
    int rightmostLimitX;
    int leftmostLimitX;

    bool hasVerticalLimit;
    int topLimitY;
    int bottomLimitY;

    bool inAir;           ///< Is it on air (due to jumping or falling).
    bool isJumping;       ///< Is it currently jumping for the first time.
    bool isDoubleJumping; ///< Is it currently jumping for the second time.
    bool win; ///< If player already reach the end of the game AND wins

    /// Ammount of strength the player has when jumping.
    ///
    /// Force applied to itself when jumping.
    float thrust;

    /// Secret mode that allows the player to fly around the
    /// scenery.
    bool flyMode;

    bool isDashing; ///< Is it currently dashing.

    bool damaging;  ///< Is it currently having damage.

    /// The movable platform that the player's currently on top.
    ///
    /// This should point to the current movable platform.
    /// It will make the player move along with it.
    ///
    /// If there is no movable platform (this is NULL), the
    /// player will behave normally.
    PlatformMovable* movablePlatform;
};

#endif //PLAYER_H_DEFINED

