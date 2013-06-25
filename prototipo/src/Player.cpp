#include <cmath> // fabs()
#include "Player.hpp"
#include "InputManager.hpp"
#include "GameStateGame.hpp"
#include "Log.hpp"

Player::Player(float x, float y, int w, int h, int hp, float acceleration):
    GameObject(x, y, w, h),
    DamageableObject(hp),
    vx(0), vy(0),
    ax(0), ay(0),
    acceleration(acceleration),
    currentAnimation(NULL),
    facingDirection(RIGHT),
    isJumping(false),
    hasHorizontalLimit(false),
    hasVerticalLimit(false)
{
    Animation* tmp = NULL;

    this->animations.resize(ANIMATION_MAX);

    int animationSpeed = 12;

    tmp = new Animation("img/apterus-standing.png", 1, 0);
    this->animations[STANDING_LEFT] = tmp;

    tmp = new Animation("img/apterus-standing-right.png", 1, 0);
    this->animations[STANDING_RIGHT] = tmp;

    tmp = new Animation("img/apterus-running.png", 6, animationSpeed);
    this->animations[RUNNING_LEFT] = tmp;

    tmp = new Animation("img/apterus-running-right.png", 6, animationSpeed);
    this->animations[RUNNING_RIGHT] = tmp;

    tmp = new Animation("img/apterus-jumping.png", 6, animationSpeed);
    this->animations[JUMPING_LEFT] = tmp;

    tmp = new Animation("img/apterus-jumping.png", 6, animationSpeed);
    this->animations[JUMPING_RIGHT] = tmp;

    this->currentAnimation = this->animations[STANDING_RIGHT];
    this->currentAnimation->start();

    this->previousBoundingBox = new RectBoundingBox(this->box->x,
                                                    this->box->y,
                                                    this->box->w,
                                                    this->box->h);
}
Player::~Player()
{ }
void Player::update(uint32_t dt)
{
    this->previousBoundingBox->copy(this->boundingBox);

    this->ax = 0;
    this->ay = 0;

    this->inputUpdate();

    // if (this->rotation < 0)
    //     this->rotation = 360 - this->rotation;

    // accelerating
    this->vx += this->ax * dt;
    this->vy += this->ay * dt;

    // deaccelerating
    this->vx *= 0.81231;
    this->vy *= 0.88918;

    this->box->addX(this->vx);
    this->box->addY(this->vy);

    // Limiting, if necessary
    if (this->hasHorizontalLimit)
    {
        if (this->box->x < this->leftmostLimitX)
            this->box->x = this->leftmostLimitX;

        if ((this->box->x + this->box->w) > this->rightmostLimitX)
            this->box->x = (this->rightmostLimitX - this->box->w);

        this->box->recalculate();
    }
    if (this->hasVerticalLimit)
    {
        if (this->box->y < this->topLimitY)
            this->box->y = this->topLimitY;

        if ((this->box->y + this->box->h) > this->bottomLimitY)
            this->box->y = (this->bottomLimitY - this->box->h);

        this->box->recalculate();
    }


    // Updating visible
    this->boundingBox->update();

    this->animationUpdate();
}
void Player::undoUpdate()
{
    this->boundingBox->copy(this->previousBoundingBox);
    this->vx = 0;
    this->vy = 0;

    Log::debug("Player::undoUpdate");
}
void Player::render(float cameraX, float cameraY)
{
   if (GameStateGame::showBoundingBoxes)
       this->boundingBox->render(cameraX, cameraY);

    this->currentAnimation->render(this->box->x - cameraX,
                                   this->box->y - cameraY);
}
void Player::inputUpdate()
{
    InputManager* input = InputManager::getInstance();

    if (input->isKeyPressed(SDLK_a))
    {
        this->facingDirection = LEFT;
        this->ax = -1 * this->acceleration;

        if (input->isKeyPressed(SDLK_LSHIFT)) // Turbo rotation
            this->ax = -1 * 2 * this->acceleration;
    }

    if (input->isKeyPressed(SDLK_d))
    {
        this->facingDirection = RIGHT;
        this->ax = this->acceleration;

        if (input->isKeyPressed(SDLK_LSHIFT)) // Turbo rotation
            this->ax = 2 * this->acceleration;
    }

    if (input->isKeyPressed(SDLK_w))
    {
        this->ay = -1 * this->acceleration;

        if (input->isKeyPressed(SDLK_LSHIFT)) // Turbo rotation
            this->ay = -1 * 2 * this->acceleration;
    }
    if (input->isKeyPressed(SDLK_s))
    {
        this->ay = this->acceleration;

        if (input->isKeyPressed(SDLK_LSHIFT)) // Turbo rotation
            this->ay =  2 * this->acceleration;
    }
}
void Player::animationUpdate()
{
    // How much of the speed do we consider as "stopped"
    float stoppedTolerance = 0.50;

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

    if (this->isJumping)
    {
        if (this->facingDirection == RIGHT)
        {
            if (this->currentAnimation != this->animations[JUMPING_RIGHT])
            {
                willChangeAnimation = true;
                tmp = this->animations[JUMPING_RIGHT];
            }
        }
        else
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
        if (fabs(this->vx) < stoppedTolerance) // it is stopped
        {
            if (this->facingDirection == RIGHT)
            {
                if (this->currentAnimation != this->animations[STANDING_RIGHT])
                {
                    willChangeAnimation = true;
                    tmp = this->animations[STANDING_RIGHT];
                }
            }
            else // facing left
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
            else // facing left
            {
                if (this->currentAnimation != this->animations[RUNNING_LEFT])
                {
                    willChangeAnimation = true;
                    tmp = this->animations[RUNNING_LEFT];
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

    int newFramerate = (int)fabs((this->vx));
    this->currentAnimation->setFramerate(newFramerate);

    this->currentAnimation->update();
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
