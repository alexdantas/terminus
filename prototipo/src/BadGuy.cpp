#include <cmath> // fabs()
#include "BadGuy.hpp"
#include "GameStateGame.hpp"
#include "Log.hpp"
#include "PhysicsManager.hpp"
#include "Config.hpp"

BadGuy::BadGuy(float x, float y, int w, int h, int hp, float acceleration):
    GameObject(x, y, w, h),
    DamageableObject(hp),
    vx(0), vy(0),
    ax(0), ay(0),
    acceleration(acceleration),
    currentAnimation(NULL),
    facingDirection(RIGHT),
    hasHorizontalLimit(false),
    hasVerticalLimit(false),
    isAttacking(false),
    dead(false),
    damaging(false)
{
    this->desiredPosition = new Rectangle();
}
BadGuy::~BadGuy()
{ }

void BadGuy::render(float cameraX, float cameraY)
{
    this->currentAnimation->render(this->position->x - cameraX,
                                   this->position->y - cameraY);
}

void BadGuy::setHorizontalLimit(int left, int right)
{
    this->leftmostLimitX  = left;
    this->rightmostLimitX = right;

    this->hasHorizontalLimit = true;
}

void BadGuy::setVerticalLimit(int top, int bottom)
{
    this->topLimitY    = top;
    this->bottomLimitY = bottom;

    this->hasVerticalLimit = true;
}

void BadGuy::updateAnimation()
{

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
    else if (this->dead)
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
    else if(this->isAttacking)
    {
        if (this->facingDirection == RIGHT)
        {
            if (this->currentAnimation != this->animations[ATTACK_RIGHT])
            {
                willChangeAnimation = true;
                tmp = this->animations[ATTACK_RIGHT];
            }
        }
        else
        {
            if (this->currentAnimation != this->animations[ATTACK_LEFT])
            {
                willChangeAnimation = true;
                tmp = this->animations[ATTACK_LEFT];
            }
        }
    }
    else //Just flying
    {
        if (this->facingDirection == RIGHT)
        {
            if (this->currentAnimation != this->animations[STANDING_RIGHT])
            {
                willChangeAnimation = true;
                tmp = this->animations[STANDING_RIGHT];
            }
        }
        else
        {
            if (this->currentAnimation != this->animations[STANDING_LEFT])
            {
                willChangeAnimation = true;
                tmp = this->animations[STANDING_LEFT];
            }
        }
    }

    if (willChangeAnimation)
    {
        this->currentAnimation->stop();
        this->currentAnimation = tmp;
        this->currentAnimation->start();
    }

    this->currentAnimation->update();
}

void BadGuy::commitMovement()
{

    this->position->x -= (this->box->x - this->desiredPosition->x);
    this->position->y -= (this->box->y - this->desiredPosition->y);

    // Refreshing next bounding box
    this->box->copy(this->desiredPosition);
}

void BadGuy::die()
{
    this->dead = true;
}

bool BadGuy::isAlive()
{
    return (!this->dead);
}

void BadGuy::dealDamage()
{
    this->damaging = true;
}

bool BadGuy::isVisible()
{
    return (this->visible);
}

void BadGuy::setVisible(bool option)
{
    this->visible = option;
}

