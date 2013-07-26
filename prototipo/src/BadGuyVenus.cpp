#include "BadGuyVenus.hpp"
#include "GameStateGame.hpp"
#include "Log.hpp"
#include "Config.hpp"

BadGuyVenus::BadGuyVenus(float x, float y, int w, int h, int hp, float acceleration):
    BadGuy(x, y, w, h, hp, acceleration)
{
    Animation* tmp = NULL;

    this->animations.resize(ANIMATION_MAX);

    int animationSpeed = 10;

    tmp = new Animation("img/spritesheets/venusstandby_esq.png", 10, animationSpeed);
    this->animations[STANDING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/venusstandby.png", 10, animationSpeed);
    this->animations[STANDING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/Untitled-6.png", 5, animationSpeed, 1);
    this->animations[DAMAGING_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/Untitled-5.png", 5, animationSpeed, 1);
    this->animations[DAMAGING_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/venus sprite atak_esq.png", 16, animationSpeed, 1);
    this->animations[ATTACK_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/venus sprite atak.png", 16, animationSpeed, 1);
    this->animations[ATTACK_RIGHT] = tmp;

    tmp = new Animation("img/spritesheets/venusmorrendosprite_esq.png", 10, animationSpeed, 1);
    this->animations[DEATH_LEFT] = tmp;

    tmp = new Animation("img/spritesheets/venusmorrendosprite.png", 10, animationSpeed, 1);
    this->animations[DEATH_RIGHT] = tmp;

    this->beam = new Thorn("img/sprites/projetilvenus.png", this->position->x + 91, this->position->y + 50, 16, 15);


    // Let's start by looking at our right.
    this->currentAnimation = this->animations[STANDING_RIGHT];

    this->isAttacking = false;

    this->timer = new TimerCounter(5000);
    this->timer->startCounting();

    this->box->stretch(0.5, 1); // Making it smaller
    this->box->addX(5); //Venus' center position
    this->desiredPosition = new Rectangle();
    this->currentAnimation->start();
}

BadGuyVenus::~BadGuyVenus()
{

}

//OHMYGOD, I FORGET HOW TO CODE
void BadGuyVenus::updateAttack(float dt)
{
    if(!this->currentAnimation->isRunning())
    {
        if(this->beam->position->x <= 800)
        {
            this->beam->addPosition(5,0);

        }
        else
        {
            this->isAttacking = false;
            this->beam->setPosition(this->position->x + 91, this->beam->position->y);
            this->currentAnimation->start();
            this->timer->restart();
        }

        this->beam->update(dt);
    }
}

void BadGuyVenus::update(float dt)
{
    this->desiredPosition->copy(this->box);

    if(this->isDead())
        this->dead = true;

    if(this->timer->isDone())
    {
        this->isAttacking = true;
    }
    else if((this->currentAnimation == this->animations[ATTACK_LEFT] || this->currentAnimation == this->animations[ATTACK_RIGHT]))
    {
        this->updateAttack(dt);
    }


    this->desiredPosition->update();

    this->updateAnimation(dt);
}

void BadGuyVenus::stepIntoMovablePlatform(PlatformMovable* platform)
{
    // Watch out for NULL poiters!
    this->movablePlatform = platform;
}
