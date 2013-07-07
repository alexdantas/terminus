#include "PlatformContainer.hpp"
#include "SDL.hpp"
#include "Log.hpp"

PlatformContainer::PlatformContainer(unsigned int maxAmmount, Rectangle areaLimit):
    maxAmmount(maxAmmount),
    currentAmmount(0),
    areaLimit(areaLimit)
{
    this->sprites.resize(PLATFORM_MAX);

    this->sprites[GROUND]    = new Sprite("img/sprites/plataforma1.png");
    this->sprites[CLOUD]     = new Sprite("img/sprites/plataforma2.png");
    this->sprites[VANISHING] = new Sprite("img/sprites/plataforma3.png");

    this->usedPlatforms.resize(0);
}
PlatformContainer::~PlatformContainer()
{
    unsigned int size = this->sprites.size();
    for (unsigned int i = 0; i < PLATFORM_MAX; i++)
        if (this->sprites[i])
            delete (this->sprites[i]);

    size = this->usedPlatforms.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (this->usedPlatforms[i])
            delete this->usedPlatforms[i];
    }
}
void PlatformContainer::add(Point p, PlatformType type)
{
    if (this->currentAmmount >= this->maxAmmount)
        return;

    Platform* tmp = new Platform(this->sprites[type],
                                 p.x, p.y,
                                 this->sprites[type]->getWidth(),
                                 this->sprites[type]->getHeight());
    Rectangle platform(p.x,
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

    if (platform.rightmost < this->areaLimit.leftmost  ||
        platform.leftmost  > this->areaLimit.rightmost ||
        platform.bottom    < this->areaLimit.top       ||
        platform.top       > this->areaLimit.bottom)
    {
        delete tmp;
        return;
    }

    this->usedPlatforms.push_back(tmp);
    this->currentAmmount++;

    Log::verbose("PlatformContainer::add (" + SDL::intToString(p.x) +
                 ", " + SDL::intToString(p.y) +
                 ") " +
                 "Count: " + SDL::intToString(this->currentAmmount));

}
void PlatformContainer::addBetween(Point a, Point b, PlatformType type)
{
    if (type == PLATFORM_MAX) // will send random types
    {
        // This is very ugly, but the only way I found of interfacing
        // between int and enums.

        int num = SDL::randomNumberBetween(static_cast<int>(GROUND),
                                           static_cast<int>(VANISHING));

        PlatformType Type = static_cast<PlatformType>(num);

        // Taking into consideration the Platform's width
        int actualX = b.x - this->sprites[Type]->getWidth();

        Point p(SDL::randomNumberBetween(a.x, actualX),
                SDL::randomNumberBetween(a.y, b.y));

        this->add(p, Type);
    }
    else
    {
        // Taking into consideration the Platform's width
        int actualX = b.x - this->sprites[type]->getWidth();

        Point p(SDL::randomNumberBetween(a.x, actualX),
                SDL::randomNumberBetween(a.y, b.y));

        this->add(p, type);
    }
}
// void PlatformContainer::addAtRandom()
// {
//     // Cannot add anyway
//     if (this->currentAmmount >= this->maxAmmount)
//         return;

//     // Will forcefully try to add until it works.
//     // Watch out for those infinite loops!

//     unsigned int previousAmmount = this->currentAmmount;

//     while (this->currentAmmount == previousAmmount)
//     {
//         int safetyCheckToAvoidInfiniteLoops = 10;


//         int x = SDL::randomNumberBetween(this->areaLimit.leftmost,
//                                          this->areaLimit.rightmost);
//         int y = SDL::randomNumberBetween(this->areaLimit.top,
//                                          this->areaLimit.bottom);

//         // This will fail silentrly if (x, y) is not inside bounds
//         this->addAt(Point(x, y));


//         if (--safetyCheckToAvoidInfiniteLoops < 0)
//             break;
//     }
// }
void PlatformContainer::addAll()
{
    // Cannot add anyway
    if (this->currentAmmount >= this->maxAmmount)
        return;

    // Will forcefully try to add until it works.
    // Watch out for those infinite loops!

    while (this->currentAmmount != this->maxAmmount)
    {
        int safetyCheckToAvoidInfiniteLoops = 10;


        int x = SDL::randomNumberBetween(this->areaLimit.leftmost,
                                         this->areaLimit.rightmost);
        int y = SDL::randomNumberBetween(this->areaLimit.top,
                                         this->areaLimit.bottom);

        // Random type
        unsigned int index = SDL::randomNumberBetween(GROUND, VANISHING);

        // This will fail quietly if (x, y) is not inside bounds
        this->add(Point(x, y), static_cast<PlatformType>(index));

        if (--safetyCheckToAvoidInfiniteLoops < 0)
            break;
    }
}
void PlatformContainer::update(float dt)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
    {
        if ((this->usedPlatforms[i]) &&
            (this->usedPlatforms[i]->isVisible()))
            this->usedPlatforms[i]->update(dt);

        // testing the limits
        Rectangle platform(this->usedPlatforms[i]->getX(),
                        this->usedPlatforms[i]->getY(),
                        this->usedPlatforms[i]->getWidth(),
                        this->usedPlatforms[i]->getHeight());

        // if (platform.rightmost < this->areaLimit.leftmost  ||
        //     platform.leftmost  > this->areaLimit.rightmost ||
        //     platform.bottom    < this->areaLimit.top       ||
        //     platform.top       > this->areaLimit.bottom)
        //     this->removeAt(i);

        // TODO
        // This has a MAJOR design flaw.
        // If I deleted a platform at position i,
        // it will get swapped to the last and removed when I call removeAt(i).
        //
        // But since i is still an index to the array, it will
        // instantly point to the next one and it will skip.
    }
}
// void PlatformContainer::removeAt(unsigned int index)
// {
//     // Double-checking to make sure I won't remove invalid stuff
//     if (this->currentAmmount == 0)
//         return;

//     unsigned int size = this->usedPlatforms.size();
//     if (index >= size)
//         return;

//     Platform* tmp = this->usedPlatforms[index];

//     std::swap(this->usedPlatforms[index], this->usedPlatforms.back());

//     Log::verbose("PlatformContainer::removeAt (" + SDL::intToString(tmp->getX()) +
//                  ", " + SDL::intToString(tmp->getY()) +
//                  ") " +
//                  "Count: " + SDL::intToString(this->currentAmmount - 1));

//     this->freePlatforms.push_back(tmp);
//     this->usedPlatforms.pop_back();
//     this->currentAmmount--;
// }
void PlatformContainer::render(float cameraX, float cameraY)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
    {
        if ((this->usedPlatforms[i]) &&
            (this->usedPlatforms[i]->isVisible()))
            this->usedPlatforms[i]->render(cameraX, cameraY);
    }
}
void PlatformContainer::limitArea(Rectangle a)
{
    this->areaLimit = a;
}
bool PlatformContainer::collidesWith(GameObject* other)
{
    if (this->usedPlatforms.size() == 0) return false;

    int size = this->usedPlatforms.size();

    for (int i = 0; i < size; i++)
        if (other->oneWayCollidedWith(this->usedPlatforms[i]))
            return true;

    return false;
}


