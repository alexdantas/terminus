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
    vx(0), vy(0),
    ax(0), ay(0),
    acceleration(acceleration),
    currentAnimation(NULL),
    facingDirection(RIGHT),
    hasHorizontalLimit(false),
    hasVerticalLimit(false),
    inAir(false),
    isJumping(false),
    isDoubleJumping(false),
    thrust(68.14159265359),
    flyMode(false),
    isDashing(false),
    dead(false),
    damaging(false)
{
    Animation* tmp = NULL;

    this->animations.resize(ANIMATION_MAX);

    int animationSpeed = Config::playerFramerate;

    tmp = new Animation("img/apterus-standing-left.png", 32, animationSpeed);
    this->animations[STANDING_LEFT] = tmp;

    tmp = new Animation("img/apterus-standing-right.png", 32, animationSpeed);
    this->animations[STANDING_RIGHT] = tmp;

    tmp = new Animation("img/apterus-running.png", 6, animationSpeed);
    this->animations[RUNNING_LEFT] = tmp;

    tmp = new Animation("img/apterus-running-right.png", 6, animationSpeed);
    this->animations[RUNNING_RIGHT] = tmp;

    tmp = new Animation("img/apterus-jumping-left.png", 6, animationSpeed);
    this->animations[JUMPING_LEFT] = tmp;

    tmp = new Animation("img/apterus-jumping-right.png", 6, animationSpeed);
    this->animations[JUMPING_RIGHT] = tmp;

    tmp = new Animation("img/apterus-dashing-left.png", 5, animationSpeed, 1);
    tmp->setTransparentRGBColor(Color(255, 255, 255));
    this->animations[DASHING_LEFT] = tmp;

    tmp = new Animation("img/apterus-dashing-right.png", 5, animationSpeed, 1);
    this->animations[DASHING_RIGHT] = tmp;

    tmp = new Animation("img/apterus-damaging-left.png", 7, animationSpeed);
    this->animations[DAMAGING_LEFT] = tmp;

    tmp = new Animation("img/apterus-damaging-right.png", 7, animationSpeed);
    this->animations[DAMAGING_RIGHT] = tmp;

    tmp = new Animation("img/apterus-death-left.png", 7, animationSpeed);
    this->animations[DEATH_LEFT] = tmp;

    tmp = new Animation("img/apterus-death-right.png", 7, animationSpeed);
    this->animations[DEATH_RIGHT] = tmp;

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

    // The acceleration is reseted each frame
    this->ax = 0;
    this->ay = 0;

    this->updateInput();
    this->updateGravity(dt);

    // accelerating
    this->vx += this->ax * dt;

    // deaccelerating for smoothness
    this->vx *= 0.81231;
    this->vy *= 0.88918;

    // actually moving the pixels on the screen
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
        {
            this->box->y = (this->bottomLimitY - this->box->h);

            // On this SPECIAL CASE, if the player hit the bottom
            // of the screen, will not jump anymore
            this->jump(false);
        }

        this->box->recalculate();
    }

    // Updating visible
    this->boundingBox->update();

    this->updateAnimation();
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
void Player::updateInput()
{
    InputManager* input = InputManager::getInstance();

    float turbo = 1.7;

    if (this->damaging)
        return;

    if (input->isKeyPressed(SDLK_a) ||
        input->isKeyPressed(SDLK_LEFT))
    {
        this->facingDirection = LEFT;
        this->ax = -1 * this->acceleration;

        if (input->isKeyPressed(SDLK_LSHIFT))
            this->ax *= turbo;
    }

    if (input->isKeyPressed(SDLK_d) ||
        input->isKeyPressed(SDLK_RIGHT))
    {
        this->facingDirection = RIGHT;
        this->ax = this->acceleration;

        if (input->isKeyPressed(SDLK_LSHIFT))
            this->ax *= turbo;
    }

    if (input->isKeyDown(SDLK_w)  ||
        input->isKeyDown(SDLK_UP) ||
        input->isKeyPressed(SDLK_z))
    {
        if (!(this->flyMode))
            this->jump(true);
    }

    if (input->isKeyPressed(SDLK_w) ||
        input->isKeyPressed(SDLK_UP))
    {
        if (this->flyMode)
        {
            this->ay = -1 * this->acceleration;

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

    // How much of the speed do we consider as "stopped"
    float stoppedTolerance = 1.90;

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
    if (willJump)
    {
        if (this->isDoubleJumping) return;

        if (this->isJumping)
            this->isDoubleJumping = true;

        this->inAir = true;
        this->isJumping = true;
        this->vy = -1 * this->thrust;
    }
    else
    {
        // cancel jumping
        if (!(this->inAir)) return;

        this->inAir = false;
        this->vy = 0;
        this->isJumping = false;
        this->isDoubleJumping = false;
    }
}
void Player::fall()
{
    this->inAir = true;
}

///Calcula a gravidade exercida no Apterus
// http://www.dreamincode.net/forums/topic/227175-c-sdl-sprite-jump/
//Se estiver no ar, então calcule a gravidade
//aceleração = somente a aceleração da Gravidade = 9.8
//velocidade = v(t) + acc
//Ou seja, é a velocidade naquele tempo t + a aceleração constante
//posição = s(t) + v(novo t)
void Player::updateGravity(uint32_t dt)
{
    if (this->flyMode)
    {
        this->vy += this->ay * dt;
    }

    float acc = 0;

    if (this->inAir)
    {
        acc += PhysicsManager::gravityAcceleration * dt/15;

        this->vy += acc;
    }
    else
        this->vy = 0;
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

