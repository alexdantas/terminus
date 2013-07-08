#include "PlatformContainer.hpp"
#include "SDL.hpp"
#include "Log.hpp"

PlatformContainer::PlatformContainer(unsigned int maxAmmount, Rectangle areaLimit):
    maxAmmount(maxAmmount),
    currentAmmount(0),
    areaLimit(areaLimit)
{
    this->sprites.resize(PLATFORM_MAX); // Maximum size of the vector

    this->sprites[GROUND]    = new Sprite("img/sprites/plataforma1.png");
    this->sprites[CLOUD]     = new Sprite("img/sprites/plataforma2.png");
    this->sprites[VANISHING] = new Sprite("img/sprites/plataforma3.png");
}
PlatformContainer::~PlatformContainer()
{
    unsigned int size = this->sprites.size();
    for (unsigned int i = 0; i < size; i++)
        if (this->sprites[i])
            delete (this->sprites[i]);

    for (std::list<Platform*>::iterator it = this->platforms.begin();
         it != this->platforms.end();
         it++)
        if (*it)
            delete (*it);
}
void PlatformContainer::add(Point p, PlatformType type)
{
    // It doesn't matter if we've reached the point of no return,
    // we'll keep adding platforms 'till the end of times.
    if (this->platforms.size() >= this->maxAmmount)
    {
        this->deleteLast();
        this->currentAmmount--;
    }

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

    this->platforms.push_back(tmp);
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
void PlatformContainer::update(float dt)
{
    for (std::list<Platform*>::iterator it = this->platforms.begin();
         it != this->platforms.end();
         it++)
    {
        if ((*it) &&
            (*it)->isVisible())
            (*it)->update(dt);
    }
}
void PlatformContainer::render(float cameraX, float cameraY)
{
    for (std::list<Platform*>::iterator it = this->platforms.begin();
         it != this->platforms.end();
         it++)
    {
        if ((*it) && (*it)->isVisible())
            (*it)->render(cameraX, cameraY);
    }
}
void PlatformContainer::limitArea(Rectangle a)
{
    this->areaLimit = a;
}
bool PlatformContainer::collidesWith(GameObject* other)
{
    int size = this->platforms.size();

    if (size == 0)
        return false;

    // Since queues cannot be iterated, will need to use a
    // temporary one on which we'll deposit all elements,
    // one by one.
    //
    // Huge performance-taker, right?

    for (std::list<Platform*>::iterator it = this->platforms.begin();
         it != this->platforms.end();
         it++)
    {
        if (other->oneWayCollidedWith(*it))
            return true;
    }
    return false;
}
Platform* PlatformContainer::getTopPlatform()
{
    if (this->platforms.empty())
        return NULL;

    return (this->platforms.back());
}
void PlatformContainer::deleteLast()
{
    // Deletes the LAST platform which is the FIRST element
    // on the queue.
    //
    // First-In-First-Out, remember?

    if (this->platforms.empty())
        return;

    Platform* tmp = this->platforms.front();

    this->platforms.pop_front();
    if (tmp) delete tmp;
}
bool PlatformContainer::isFull()
{
    return (this->currentAmmount == this->maxAmmount);
}

