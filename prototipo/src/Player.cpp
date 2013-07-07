#include <cmath> // fabs()
#include "Player.hpp"
#include "InputManager.hpp"
#include "GameStateGame.hpp"
#include "Log.hpp"
#include "PhysicsManager.hpp"
#include "Config.hpp"

Player::Player(float x, float y, int w, int h, int hp, float acceleration):
    GameObject(x, y, w, h),
    DamageableObject(hp),
    desiredPosition(NULL),
    vx(0), vy(0),
    ax(0), ay(0),
    acceleration(acceleration),
    stoppedThreshold(acceleration/5.5), // 10% of the speed
    currentAnimation(NULL),
    facingDirection(RIGHT),
    hasHorizontalLimit(false),
    hasVerticalLimit(false),
    inAir(false),
    isJumping(false),
    isDoubleJumping(false),
    thrust(Config::playerJump),
    flyMode(false),
    isDashing(false),
    dead(false),
    damaging(false)
{
    Animation* tmp = NULL;

    this->animations.resize(ANIMATION_MAX);

    int animationSpeed = Config::playerFramerate;

    tmp = new Animation("img/spritesheets/apterus-standing-left.png", 32, animationSpeed);
    this->animations[STANDING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-standing-right.png", 32, animationSpeed);
    this->animations[STANDING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-running.png", 6, animationSpeed);
    this->animations[RUNNING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-running-right.png", 6, animationSpeed);
    this->animations[RUNNING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-jumping-left.png", 6, animationSpeed);
    this->animations[JUMPING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-jumping-right.png", 6, animationSpeed);
    this->animations[JUMPING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-dashing-left.png", 5, animationSpeed, 1);
    tmp->setTransparentRGBColor(Color(255, 255, 255));
    this->animations[DASHING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-dashing-right.png", 5, animationSpeed, 1);
    this->animations[DASHING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-damaging-left.png", 7, animationSpeed);
    this->animations[DAMAGING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-damaging-right.png", 7, animationSpeed);
    this->animations[DAMAGING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-death-left.png", 7, animationSpeed);
    this->animations[DEATH_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/apterus-death-right.png", 7, animationSpeed);
    this->animations[DEATH_RIGHT] = tmp;

    // Let's start by looking at our right.
    this->currentAnimation = this->animations[STANDING_RIGHT];
    this->currentAnimation->start();

    // Correcing bounding box:
    // Now I'm changing the bounding box's position INDEPENDENTLY
    // OF THE SPRITE.
    this->box->stretch(0.5, 0.8); // Making it smaller
    this->box->addX(5);           // Walk it a little to the right
    this->box->addY(20);          // And a little down
    this->desiredPosition = new Rectangle();
}
Player::~Player()
{ }
void Player::update(float dt)
{
    this->desiredPosition->copy(this->box);

    // The acceleration is reseted each frame
    this->ay = 0;
    this->targetVx = 0;

    // These will define the resulting acceleration
    // (adding all the forces - input force, gravity force, etc)
    this->updateInput();

    // VERTICAL MOVEMENT
//    if (this->flyMode)
//        return;

    // Player will ALWAYS suffer gravity.
    // The collision resolution scheme is resposible for not
    // letting the player go through stuff.
    this->vy += (PhysicsManager::gravityAcceleration * dt);

    // HORIZONTAL MOVEMENT
    // Acceleration rate: How fast the player hits the
    //                    full speed.
    //
    // 1: Instant Full-Speed
    // 0: Never Moves
    float a = 0.2612312321;

    // Method 1 of accelerating
    //
    // http://higherorderfun.com/blog/2012/05/20/the-guide-to-implementing-2d-platformers/
    this->vx = (a * this->targetVx) + ((1 - a) * this->vx) * dt;

    // Method 2 of accelerating
    //
    // http://earok.net/sections/articles/game-dev/theory/simplified-acceleration-games
    // this->vx += ((this->targetVx - this->vx) * a) * dt;

    // Making sure we will not be sliding indefinitely,
    // stopping if we hit a certain low speed.
    if (fabs(this->vx) < this->stoppedThreshold)
        this->vx = 0;

    // deaccelerating for smoothness
//    this->vx *= PhysicsManager::groundFriction;
//    this->vy *= PhysicsManager::airFriction;

    // actually moving the pixels on the screen
    this->desiredPosition->addX(this->vx);
    this->desiredPosition->addY(this->vy);

    // Limiting, if necessary
    if (this->hasHorizontalLimit)
    {
        if (this->desiredPosition->x < this->leftmostLimitX)
            this->desiredPosition->x = this->leftmostLimitX;

        if ((this->desiredPosition->x + this->desiredPosition->w) > this->rightmostLimitX)
            this->desiredPosition->x = (this->rightmostLimitX - this->desiredPosition->w);

        this->desiredPosition->update();
    }
    if (this->hasVerticalLimit)
    {
        if (this->desiredPosition->y < this->topLimitY)
            this->desiredPosition->y = this->topLimitY;

        if ((this->desiredPosition->y + this->desiredPosition->h) >= this->bottomLimitY)
        {
            this->desiredPosition->y = (this->bottomLimitY - this->desiredPosition->h);

            // On this SPECIAL CASE, if the player hit the bottom
            // of the screen, will not jump anymore.
            this->jump(false);
        }

        this->desiredPosition->update();
    }

    // Updating visible
    this->desiredPosition->update();

    this->updateAnimation();
}
void Player::render(float cameraX, float cameraY)
{
    this->currentAnimation->render(this->position->x - cameraX,
                                   this->position->y - cameraY);
}
void Player::updateInput()
{
    InputManager* input = InputManager::getInstance();

    float turbo = 1.7;

    if (this->damaging)
        return;

    if (input->isKeyPressed(SDLK_a) ||
        input->isKeyPressed(SDLK_LEFT))
    {
        this->targetVx = (-1 * this->acceleration);
        this->facingDirection = LEFT;

        if (input->isKeyPressed(SDLK_LSHIFT))
            this->targetVx *= turbo;
    }

    if (input->isKeyPressed(SDLK_d) ||
        input->isKeyPressed(SDLK_RIGHT))
    {
        this->targetVx = (this->acceleration);
        this->facingDirection = RIGHT;

        if (input->isKeyPressed(SDLK_LSHIFT))
            this->targetVx *= turbo;
    }

    if (input->isKeyDown(SDLK_w)  ||
        input->isKeyDown(SDLK_UP) ||
        input->isKeyPressed(SDLK_z))
    {
        if (!(this->flyMode))
            this->jump();
    }

    if (input->isKeyPressed(SDLK_w) ||
        input->isKeyPressed(SDLK_UP))
    {
        if (this->flyMode)
        {
            this->ay = (-1 * this->acceleration);

            if (input->isKeyPressed(SDLK_LSHIFT)) // Turbo rotation
                this->ay = -1 * 2 * this->acceleration;
        }
    }
    if (input->isKeyPressed(SDLK_s) ||
        input->isKeyPressed(SDLK_DOWN))
    {
        if (this->flyMode)
        {
            this->ay = this->acceleration;

            if (input->isKeyPressed(SDLK_LSHIFT)) // Turbo rotation
                this->ay =  2 * this->acceleration;
        }
    }
    if (input->isKeyDown(SDLK_SPACE) ||
        input->isKeyDown(SDLK_x))
    {
        this->dash();
    }

    if (input->isKeyDown(SDLK_DELETE))
    {
        this->die();
    }

    if (input->isKeyDown(SDLK_BACKSPACE))
        this->dealDamage();

    // TODO TMP TEMP
    if (input->isKeyDown(SDLK_o))
        PhysicsManager::gravityAcceleration += 0.5;
    if (input->isKeyDown(SDLK_i))
        PhysicsManager::gravityAcceleration -= 0.5;
}
void Player::updateAnimation()
{
    // Since we dash until the animation stops, makes sense to check
    // if we'll stop dashing here
    if (this->isDashing)
    {
        if (!(this->currentAnimation->isRunning()))
            this->isDashing = false;
    }

    // These will make transitions a lot easier
    bool willChangeAnimation = false;
    Animation* tmp = NULL;

    // I *know* this is fucking plain bad-coding,
    // but I haven't thought of a cleaner way of expressing this.
    //
    // I mean, there's a lot of animations and I must check
    // at each frame if it's going to change.
    //
    // And there's a whole tree of possible animations depending
    // on a lot of circumstances... Damn, dude.

    if (this->damaging)
    {
        if (this->facingDirection == RIGHT)
        {
            if (this->currentAnimation != this->animations[DAMAGING_RIGHT])
            {
                willChangeAnimation = true;
                tmp = this->animations[DAMAGING_RIGHT];
            }
        }
        else
        {
            if (this->currentAnimation != this->animations[DAMAGING_LEFT])
            {
                willChangeAnimation = true;
                tmp = this->animations[DAMAGING_LEFT];
            }
        }
        if (this->damaging)
        {
            if (!(this->currentAnimation->isRunning()))
                this->damaging = false;
        }
    }
    if (this->dead)
    {
        if (this->facingDirection == RIGHT)
        {
            if (this->currentAnimation != this->animations[DEATH_RIGHT])
            {
                willChangeAnimation = true;
                tmp = this->animations[DEATH_RIGHT];
            }
        }
        else
        {
            if (this->currentAnimation != this->animations[DEATH_LEFT])
            {
                willChangeAnimation = true;
                tmp = this->animations[DEATH_LEFT];
            }
        }
    }
    else // NOT dead
    {
        if (this->inAir)
        {
            if (this->facingDirection == RIGHT)
            {
                if (this->currentAnimation != this->animations[JUMPING_RIGHT])
                {
                    willChangeAnimation = true;
                    tmp = this->animations[JUMPING_RIGHT];
                }
            }
            else // facingDirection == LEFT
            {
                if (this->currentAnimation != this->animations[JUMPING_LEFT])
                {
                    willChangeAnimation = true;
                    tmp = this->animations[JUMPING_LEFT];
                }
            }
        }
        else // is not jumping
        {
            if (this->isDashing)
            {
                if (this->facingDirection == RIGHT)
                {
                    if (this->currentAnimation != this->animations[DASHING_RIGHT])
                    {
                        willChangeAnimation = true;
                        tmp = this->animations[DASHING_RIGHT];
                    }
                }
                else // facingDirection == LEFT
                {
                    if (this->currentAnimation != this->animations[DASHING_LEFT])
                    {
                        willChangeAnimation = true;
                        tmp = this->animations[DASHING_LEFT];
                    }
                }
            }
            if (fabs(this->vx) < this->stoppedThreshold) // it is stopped
            {
                if (this->facingDirection == RIGHT)
                {
                    if (this->currentAnimation != this->animations[STANDING_RIGHT])
                    {
                        willChangeAnimation = true;
                        tmp = this->animations[STANDING_RIGHT];
                    }
                }
                else // facingDirection == LEFT
                {
                    if (this->currentAnimation != this->animations[STANDING_LEFT])
                    {
                        willChangeAnimation = true;
                        tmp = this->animations[STANDING_LEFT];
                    }
                }
            }
            else // it's running
            {
                if (this->facingDirection == RIGHT)
                {
                    if (this->currentAnimation != this->animations[RUNNING_RIGHT])
                    {
                        willChangeAnimation = true;
                        tmp = this->animations[RUNNING_RIGHT];
                    }
                }
                else // facingDirection == LEFT
                {
                    if (this->currentAnimation != this->animations[RUNNING_LEFT])
                    {
                        willChangeAnimation = true;
                        tmp = this->animations[RUNNING_LEFT];
                    }
                }
            }
        }
    }

    // *phew*

    if (willChangeAnimation)
    {
        this->currentAnimation->stop();
        this->currentAnimation = tmp;
        this->currentAnimation->start();
    }

    // this was FUCKING UP EVERYTHING
    // it just wouldn't work right
    //
    // i must find a way to get the running animation to change
    // according to the speed
    //
    // while that day never comes, FUCK THIS

    // int newFramerate = (int)fabs((this->vx));
    // this->currentAnimation->setFramerate(newFramerate);

    this->currentAnimation->update();
}
void Player::commitMovement()
{
    // Refreshing position
    this->position->x -= (this->box->x - this->desiredPosition->x);
    this->position->y -= (this->box->y - this->desiredPosition->y);

    // Refreshing next bounding box
    this->box->copy(this->desiredPosition);
}
void Player::setHorizontalLimit(int left, int right)
{
    this->leftmostLimitX  = left;
    this->rightmostLimitX = right;

    this->hasHorizontalLimit = true;
}
void Player::setVerticalLimit(int top, int bottom)
{
    this->topLimitY    = top;
    this->bottomLimitY = bottom;

    this->hasVerticalLimit = true;
}
void Player::jump(bool willJump)
{
    if (willJump) // Yay, let's jump!
    {
        if (this->isDoubleJumping) return;

        if (this->isJumping)
            this->isDoubleJumping = true;

        this->inAir     = true;
        this->isJumping = true;

        // Trampoline mode: If you press the jump button twice
        //                  in a row it will jump higher than
        //                  pressing once and then later twice.
        // this->vy += (-1 * this->thrust);
        this->vy = (-1 * this->thrust);
    }
    else // Will cancel jumping
    {
        this->inAir     = false;
        this->vy        = 0;
        this->isJumping = false;
        this->isDoubleJumping = false;
    }
}
void Player::fall()
{
    this->inAir = true;
}
void Player::toggleFlyMode()
{
    this->flyMode?
        this->flyMode = false:
        this->flyMode = true;
}
void Player::dash()
{
    if (this->inAir) return;
    if (this->isDashing) return;

    this->isDashing = true;

    if (this->facingDirection == RIGHT)
        this->ax =  7 * this->acceleration;
    else
        this->ax = -7 * this->acceleration;
}
void Player::die()
{
    this->dead = true;
}
bool Player::isAlive()
{
    return (!this->dead);
}
void Player::dealDamage()
{
    this->damaging = true;
}

