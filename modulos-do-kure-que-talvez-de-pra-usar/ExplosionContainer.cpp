#include "ExplosionContainer.hpp"
#include "SDL.hpp"
#include "Log.hpp"

ExplosionContainer::ExplosionContainer(unsigned int maxAmmount, Rectangle areaLimit):
    maxAmmount(maxAmmount),
    currentAmmount(0),
    areaLimit(areaLimit)
{
    this->sprites.resize(EXPLOSION_MAX);
    this->sprites[EXPLOSION_1] = new Sprite("img/explosion.png");
    this->sprites[EXPLOSION_2] = new Sprite("img/explosion.png");

    this->usedExplosions.resize(0);
    this->freeExplosions.resize(this->maxAmmount);

    for (unsigned int i = 0; i < (this->maxAmmount); i++)
    {
        unsigned int index = SDL::randomNumberBetween(EXPLOSION_1, EXPLOSION_2);

        Explosion* tmp = new Explosion(0, 0, 0.2, this->sprites[index], i);
        this->freeExplosions.push_back(tmp);
    }
}
ExplosionContainer::~ExplosionContainer()
{
    unsigned int size = this->sprites.size();
    for (unsigned int i = 0; i < EXPLOSION_MAX; i++)
        if (this->sprites[i])
            delete (this->sprites[i]);

    size = this->freeExplosions.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (this->freeExplosions[i])
            delete this->freeExplosions[i];
    }

    size = this->usedExplosions.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (this->usedExplosions[i])
            delete this->usedExplosions[i];
    }
}
void ExplosionContainer::addAt(Point p)
{
    if (this->freeExplosions.size() == 0)
        return;

    if (this->currentAmmount >= this->maxAmmount)
        return;

    Explosion* tmp = this->freeExplosions.back();

    Rectangle explosion(p.x,
                    p.y,
                    tmp->getWidth(),
                    tmp->getHeight());

    // Testing the limits
    //
    // Will try to include it like this:
    //      _______
    //  ___|       |___
    // |___|       |___|
    //     |_______|
    //
    // Those two tiny rectangles are the bounding checks
    // based on the big rectangle area.

    if (explosion.rightmost < this->areaLimit.leftmost  ||
        explosion.leftmost  > this->areaLimit.rightmost ||
        explosion.bottom    < this->areaLimit.top       ||
        explosion.top       > this->areaLimit.bottom)
        return;

    // "Creating" and adjusting
    tmp->setPosition(p);

    float speed = ((float)(SDL::randomNumberBetween(1, 5)))/30.0;

    // Randomly deciding if the explosion's gonna go to the left or right
    ((SDL::randomNumberBetween(1, 10)) % 2) == 0?
        speed *= -1:
        speed *=  1;

    tmp->setSpeed(speed);

    tmp->setVisible(true);

    this->currentAmmount++;

    this->usedExplosions.push_back(tmp);
    this->freeExplosions.pop_back();

    Log::verbose("ExplosionContainer::addAt (" + SDL::intToString(p.x) +
                 ", " + SDL::intToString(p.y) +
                 ") " +
                 "Count: " + SDL::intToString(this->currentAmmount));

}
void ExplosionContainer::addAtRandom()
{
    // Cannot add anyway
    if (this->currentAmmount >= this->maxAmmount)
        return;

    // Will forcefully try to add until it works.
    // Watch out for those infinite loops!

    unsigned int previousAmmount = this->currentAmmount;

    while (this->currentAmmount == previousAmmount)
    {
        int safetyCheckToAvoidInfiniteLoops = 10;


        int x = SDL::randomNumberBetween(this->areaLimit.leftmost,
                                         this->areaLimit.rightmost);
        int y = SDL::randomNumberBetween(this->areaLimit.top,
                                         this->areaLimit.bottom);

        // This will fail silentrly if (x, y) is not inside bounds
        this->addAt(Point(x, y));


        if (--safetyCheckToAvoidInfiniteLoops < 0)
            break;
    }
}
void ExplosionContainer::update(uint32_t dt)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
    {
        if ((this->usedExplosions[i]) &&
            (this->usedExplosions[i]->isVisible()))
            this->usedExplosions[i]->update(dt);

        // testing the limits
        Rectangle explosion(this->usedExplosions[i]->getX(),
                        this->usedExplosions[i]->getY(),
                        this->usedExplosions[i]->getWidth(),
                        this->usedExplosions[i]->getHeight());

        if (explosion.rightmost < this->areaLimit.leftmost  ||
            explosion.leftmost  > this->areaLimit.rightmost ||
            explosion.bottom    < this->areaLimit.top       ||
            explosion.top       > this->areaLimit.bottom)
            this->removeAt(i);

        // TODO
        // This has a MAJOR design flaw.
        // If I deleted a explosion at position i,
        // it will get swapped to the last and removed when I call removeAt(i).
        //
        // But since i is still an index to the array, it will
        // instantly point to the next one and it will skip.
    }
}
void ExplosionContainer::removeAt(unsigned int index)
{
    // Double-checking to make sure I won't remove invalid stuff
    if (this->currentAmmount == 0)
        return;

    unsigned int size = this->usedExplosions.size();
    if (index >= size)
        return;

    Explosion* tmp = this->usedExplosions[index];

    std::swap(this->usedExplosions[index], this->usedExplosions.back());

    Log::verbose("ExplosionContainer::removeAt (" + SDL::intToString(tmp->getX()) +
                 ", " + SDL::intToString(tmp->getY()) +
                 ") " +
                 "Count: " + SDL::intToString(this->currentAmmount - 1));

    this->freeExplosions.push_back(tmp);
    this->usedExplosions.pop_back();
    this->currentAmmount--;
}
void ExplosionContainer::render(float cameraX, float cameraY)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
    {
        if ((this->usedExplosions[i]) &&
            (this->usedExplosions[i]->isVisible()))
            this->usedExplosions[i]->render(cameraX, cameraY);
    }
}
void ExplosionContainer::limitArea(Rectangle a)
{
    this->areaLimit = a;
}

