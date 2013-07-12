#include "BadGuyVenus.hpp"
#include "GameStateGame.hpp"
#include "Log.hpp"
#include "Config.hpp"

BadGuyVenus::BadGuyVenus(float x, float y, int w, int h, int hp, float acceleration):
    BadGuy(x, y, w, h, hp, acceleration)
{
         Animation* tmp = NULL;

    this->animations.resize(ANIMATION_MAX);

    int animationSpeed = Config::playerFramerate;

    tmp = new Animation("img/spritesheets/venusstandby_esq.png", 10, animationSpeed);
    this->animations[STANDING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/venusstandby.png", 10, animationSpeed);
    this->animations[STANDING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/Untitled-6.png", 5, animationSpeed);
    this->animations[DAMAGING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/Untitled-5.png", 5, animationSpeed);
    this->animations[DAMAGING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/venus sprite atak_esq.png", 16, animationSpeed, 1);
    this->animations[ATTACK_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/venus sprite atak.png", 16, animationSpeed, 1);
    this->animations[ATTACK_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/venusmorrendosprite_esq.png", 10, animationSpeed, 1);
    this->animations[DEATH_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/venusmorrendosprite.png", 10, animationSpeed, 1);
    this->animations[DEATH_RIGHT] = tmp;

    // Let's start by looking at our right.
    this->currentAnimation = this->animations[STANDING_RIGHT];
    this->currentAnimation->start();
    this->isAttacking = false;

    this->timer = new TimerCounter(5000);
    this->timer->startCounting();
}

BadGuyVenus::~BadGuyVenus()
{

}

void BadGuyVenus::update(float dt)
{
     this->desiredPosition->copy(this->box);

    // The acceleration is reseted each frame
    this->ay = 0;
    this->targetVx = 0;

    if(this->timer->isDone())
    {
        this->isAttacking = true;
        this->timer->startCounting();
    }
    else
        this->isAttacking = false;

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
    vx = (a * this->targetVx) + ((1 - a) * this->vx) * dt;

    // actually moving the pixels on the screen
    this->desiredPosition->addX(this->vx);

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

        }

        this->updateAnimation();
    }

    this->desiredPosition->update();
    this->updateAnimation();
}

void BadGuyVenus::stepIntoMovablePlatform(PlatformMovable* platform)
{
    // Watch out for NULL poiters!
    this->movablePlatform = platform;
}
