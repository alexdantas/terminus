#include "PlatformContainer.hpp"
#include "SDL.hpp"
#include "Log.hpp"
#include "PlatformMovable.hpp"
#include "PlatformVanishing.hpp"

PlatformContainer::PlatformContainer(unsigned int maxAmmount, Rectangle areaLimit):
    maxAmmount(maxAmmount),
    currentAmmount(0),
    areaLimit(areaLimit)
{
    this->sprites.resize(Platform::MAX); // Maximum size of the vector

    this->sprites[Platform::GROUND]    = new Sprite("img/sprites/platform-1.png");
    this->sprites[Platform::CLOUD]     = new Sprite("img/sprites/platform-2.png");
    this->sprites[Platform::VANISHING] = new Sprite("img/sprites/platform-3.png");
    this->sprites[Platform::MOVABLE]   = new Sprite("img/sprites/platform-4.png");
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
void PlatformContainer::add(Point p, Platform::PlatformType type)
{
    // It doesn't matter if we've reached the point of no return,
    // we'll keep adding platforms 'till the end of times.
    if (this->platforms.size() >= this->maxAmmount)
    {
        this->deleteLast();
        this->currentAmmount--;
    }

    Platform* tmp = NULL;

    // Now we'll create a platform based on it's type.
    // Since every kind of platform is a *Platform*, we will store
    // them on a container with pointers to *Platform*.
    //
    // (this is called _upcasting_)
    if (type == Platform::MOVABLE)
    {
        PlatformMovable* tmp2 = NULL;

        tmp2 = new PlatformMovable(this->sprites[Platform::MOVABLE],
                                   p.x, p.y,
                                   this->sprites[Platform::MOVABLE]->getWidth(),
                                   this->sprites[Platform::MOVABLE]->getHeight(),
                                   SDL::randomNumberBetween(130, 330));

        tmp2->setHorizontalLimit(this->areaLimit.leftmost,
                                 this->areaLimit.rightmost);

        // upcasting is magic!
        tmp = (Platform*)tmp2;

        /// TODO BUG WARNING
        /// There's something wrong right here.
        ///
        /// I have a *PlatformMovable* with *type* equal to *Platform::MOVABLE*.
        ///
        /// Whenever I assign it to a pointer of *Platform*, it's
        /// *type* gets reset to *Platform::GROUND*.
        ///
        /// Is it because *Platform::GROUND* is zero or because it
        /// gets the type of the parent class?
        ///
        /// Anyway, that makes me force the type when assigning.
        /// Just like I did right here:
        tmp->type = Platform::MOVABLE;
    }
    else if (type == Platform::VANISHING)
    {
        PlatformVanishing* tmp2 = NULL;

        tmp2 = new PlatformVanishing(this->sprites[Platform::VANISHING],
                                     p.x, p.y,
                                     this->sprites[Platform::VANISHING]->getWidth(),
                                     this->sprites[Platform::VANISHING]->getHeight(),
                                     500); // half a second


        // WARNING The same upcasting problem as movable platforms.
        //         Check out that big chunk of comments right above.
        tmp = (Platform*)tmp2;
        tmp->type = Platform::VANISHING;
    }
    else
    {
        /// Things are simple when you'll just spawn a regular *Platform*.
        tmp = new Platform(this->sprites[type],
                           p.x, p.y,
                           this->sprites[type]->getWidth(),
                           this->sprites[type]->getHeight());
    }

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
void PlatformContainer::addBetween(Point a, Point b, Platform::PlatformType type)
{
    if (type == Platform::MAX) // will send random types
    {
        // This is very ugly, but the only way I found of interfacing
        // between int and enums.

        int num = SDL::randomNumberBetween(static_cast<int>(Platform::GROUND),
                                           static_cast<int>(Platform::MOVABLE));

        Platform::PlatformType Type = static_cast<Platform::PlatformType>(num);

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

        // If the platform is not visible, delete it and erase from
        // the list.
        if (!((*it)->isVisible()))
        {
            // Remember that the *erase()* method from std::list
            // returns the iterator of the next element right after
            // the erased one.
            //
            // Since it points to the next we decrease it because
            // the *for* loop will increase it again.
            Platform* tmp = (*it);

            it = this->platforms.erase(it);
            delete tmp;
            it--;
        }
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
// bool PlatformContainer::collidesWith(GameObject* other)
// {
//     int size = this->platforms.size();

//     if (size == 0)
//         return false;

//     // Since queues cannot be iterated, will need to use a
//     // temporary one on which we'll deposit all elements,
//     // one by one.
//     //
//     // Huge performance-taker, right?

//     for (std::list<Platform*>::iterator it = this->platforms.begin();
//          it != this->platforms.end();
//          it++)
//     {
//         if (other->oneWayCollidedWith(*it))
//             return true;
//     }
//     return false;
// }
Platform* PlatformContainer::getTopPlatform()
{
    if (this->platforms.empty())
        return NULL;

    return (this->platforms.back());
}

Platform* PlatformContainer::getBottomPlatform()
{
    if (this->platforms.empty())
        return NULL;

    return (this->platforms.front());
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

