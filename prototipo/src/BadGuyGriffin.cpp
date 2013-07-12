#include "BadGuyGriffin.hpp"
#include "GameStateGame.hpp"
#include "Log.hpp"
#include "Config.hpp"

BadGuyGriffin::BadGuyGriffin(float x, float y, int w, int h, int hp, float acceleration):
    BadGuy(x, y, w, h, hp, acceleration)
{
     Animation* tmp = NULL;

    this->animations.resize(ANIMATION_MAX);

    int animationSpeed = Config::playerFramerate;

    tmp = new Animation("img/spritesheets/frames grifo standby1.png", 18, animationSpeed);
    this->animations[STANDING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/frames grifo standby1.png", 18, animationSpeed);
    this->animations[STANDING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/frames grifo trans direita.png", 30, animationSpeed, 1);
    this->animations[DAMAGING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/frames grifo trans.png", 30, animationSpeed, 1);
    this->animations[DAMAGING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/grifo rasante direita0000.png", 1, animationSpeed, 1);
    this->animations[ATTACK_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/grifo rasante 10000.png", 1, animationSpeed, 1);
    this->animations[ATTACK_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/grifomorrendo frames esquerda.png", 22, animationSpeed, 1);
    this->animations[DEATH_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/grifomorrendo frames.png", 22, animationSpeed, 1);
    this->animations[DEATH_RIGHT] = tmp;

    // Let's start by looking at our right.
    this->currentAnimation = this->animations[ATTACK_RIGHT];
    this->currentAnimation->start();
    this->isAttacking = false;
    this->goDown = false;

    this->timer = new TimerCounter(1000);
    this->timer->startCounting();
}

BadGuyGriffin::~BadGuyGriffin()
{

}

void BadGuyGriffin::update(float dt)
{
    this->desiredPosition->copy(this->box);

    // The acceleration is reseted each frame
    this->ay = 0;
    this->targetVx = 0;

    if((this->desiredPosition->y + this->desiredPosition->h) <= this->topLimitY)
        this->goDown = true;
    else if((this->desiredPosition->y + this->desiredPosition->h) >= this->bottomLimitY)
        this->goDown = false;

    if(!goDown)
    {
        this->targetVy = (-1 * this->acceleration/2);
        this->isAttacking = false;
    }
    else
    {
        this->isAttacking = true;
        this->targetVy = (2 * this->acceleration);

        if(this->desiredPosition->x >= rightmostLimitX)
            this->facingDirection = LEFT;

        else if(this->desiredPosition->x <= leftmostLimitX)
            this->facingDirection = RIGHT;


        if(facingDirection == RIGHT)
            targetVx = (1 * this->acceleration);
        else
            targetVx = (-1 * this->acceleration);
    }

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
    vy = (a * this->targetVy) + ((1 - a) * this->vy) * dt;

    // actually moving the pixels on the screen
    this->desiredPosition->addY(this->vy);
    this->desiredPosition->addX(this->vx);


    this->desiredPosition->update();
    this->updateAnimation();
}
