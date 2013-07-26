#include "BadGuyGriffin.hpp"
#include "GameStateGame.hpp"
#include "Log.hpp"
#include "Config.hpp"
#include "Graphics.hpp"

BadGuyGriffin::BadGuyGriffin(float x, float y, int w, int h, int hp, float acceleration):
    BadGuy(x, y, w, h, hp, acceleration)
{
    this->beam = NULL;

     Animation* tmp = NULL;

    this->animations.resize(ANIMATION_MAX);

    int animationSpeed = 30;

    tmp = new Animation("img/spritesheets/grifo standby redem.png", 18, animationSpeed);
    this->animations[STANDING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/grifo standby redem.png", 18, animationSpeed);
    this->animations[STANDING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/griffo transition redem direita.png", 30, animationSpeed, 1);
    this->animations[DAMAGING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/griffo transition redem esquerda.png", 30, animationSpeed, 1);
    this->animations[DAMAGING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/grifo rasante redem direita.png", 1, animationSpeed, 1);
    this->animations[ATTACK_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/grifo rasante redem esquerda.png", 1, animationSpeed, 1);
    this->animations[ATTACK_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/grifo morrendo redem esquerda.png", 22, animationSpeed, 1);
    this->animations[DEATH_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/grifo morrendo redem.png", 22, animationSpeed, 1);
    this->animations[DEATH_RIGHT] = tmp;

    // Let's start by looking at our right.
    this->currentAnimation = this->animations[ATTACK_RIGHT];

    this->isAttacking = false;
    this->goDown = false;

    this->box->stretch(0.2, 0.7); // Making it smaller
    this->box->addY(33); //Just the Griffin body, not the wings
    //when attacks, not perfect collision box, but it's works! :D
    this->desiredPosition = new Rectangle();
    this->currentAnimation->start();

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
    this->targetVy = 0;
    this->vy = 0;

    if(!this->damaging && !this->isDead())
    {
     if((this->desiredPosition->y + this->desiredPosition->h) <= this->topLimitY)
            this->goDown = true;
        else if((this->desiredPosition->y + this->desiredPosition->h) >= this->bottomLimitY)
            this->goDown = false;

        if(!goDown)
        {
            this->targetVy = (-1 * this->acceleration);
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
    }

    else if(this->damaging)
    {
         if(facingDirection == RIGHT)
            targetVx = 15;
        else
            targetVx = -15;
    }
    else
        this->dead = true;



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
    vx = (a * this->targetVx);
    vy = (a * this->targetVy) + ((1 - a) * this->vy) * dt;

    // actually moving the pixels on the screen
    this->desiredPosition->addY(this->vy);
    this->desiredPosition->addX(this->vx);

    this->desiredPosition->update();
    this->updateAnimation(dt);

}
