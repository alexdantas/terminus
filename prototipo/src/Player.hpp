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
    void inputUpdate();

    /// Updates internal animation.
    void animationUpdate();

    void setHorizontalLimit(int left, int right);
    void setVerticalLimit(int top, int bottom);

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

    /// Tells if the player's jumping.
    bool isJumping;

    bool hasHorizontalLimit;
    int rightmostLimitX;
    int leftmostLimitX;

    bool hasVerticalLimit;
    int topLimitY;
    int bottomLimitY;

};

#endif //PLAYER_H_DEFINED

