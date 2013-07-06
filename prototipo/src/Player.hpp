#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED

#include <vector>
#include "GameObject.hpp"
#include "DamageableObject.hpp"
#include "Animation.hpp"

/// Defines the player.
class Player: public GameObject,
              public DamageableObject
{
public:
    enum FacingDirection { LEFT, RIGHT };
    enum PossibleAnimation
    {
        JUMPING_LEFT=0, JUMPING_RIGHT,
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

    void update(uint32_t dt);

    /// Returns to it's position before *update()*.
    ///
    /// This is useful in case of collisions.
    ///
    /// @note It only goes back to _this frame's_ *update()*!
    void undoUpdate();

    /// Shows onscreen.
    void render(float cameraX, float cameraY);

    /// Respond to any input commands.
    void updateInput();

    /// Updates internal animation.
    void updateAnimation();

    /// Updates y velocity if it's on air (jumping or falling).
    void updateGravity(uint32_t dt);

    void setHorizontalLimit(int left, int right);
    void setVerticalLimit(int top, int bottom);

    /// Makes the player jump.
    ///
    /// Internally it assures that it won't jump again.
    void jump(bool willJump);

    /// Forces the player to suffer gravity.
    void fall();

    /// Toggles _cheat_ fly mode, that allows the player to freely
    /// fly around there.
    void toggleFlyMode();

    /// Makes the player dash, possibly giving damage to others.
    void dash();

    /// Tells if the player's dead.
    bool isAlive();

    void die();

    void dealDamage();

private:
    float vx; ///< Speed component of the x axis.
    float vy; ///< Speed component of the y axis.

    float ax; ///< Acceleration component of the x axis.
    float ay; ///< Acceleration component of the y axis.

    float acceleration;

    /// Player's current animation.
    Animation* currentAnimation;

    /// All possible animations, ready to be selected.
    std::vector<Animation*> animations;

    /// The facing direction of the player.
    FacingDirection facingDirection;

    /// Tells if the player's limited by some distance.
    bool hasHorizontalLimit;
    int rightmostLimitX;
    int leftmostLimitX;

    bool hasVerticalLimit;
    int topLimitY;
    int bottomLimitY;

    /// Tells if the player's floating on air (due to jumping
    /// or falling.
    bool inAir;

    bool isJumping;
    bool isDoubleJumping;

    /// Ammount of strength the player has when jumping.
    float thrust;

    /// Secret mode that allows the player to fly around the
    /// scenery.
    bool flyMode;

    /// Tells about the player's dashing action.
    bool isDashing;

    bool dead;

    bool damaging;
};

#endif //PLAYER_H_DEFINED

