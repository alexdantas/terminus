#include "CloudContainer.hpp"
#include "SDL.hpp"
#include "Log.hpp"

CloudContainer::CloudContainer(unsigned int maxAmmount, Rectangle areaLimit, bool blackClouds):
    maxAmmount(maxAmmount),
    currentAmmount(0),
    areaLimit(areaLimit),
    timer(NULL)
{
    this->sprites.resize(CLOUD_MAX);

    // HACK to make black/white clouds available
    if (blackClouds)
    {
        this->sprites[CLOUD_1] = new Sprite("img/sprites/cloud-1.png");
        this->sprites[CLOUD_2] = new Sprite("img/sprites/cloud-2.png");
        this->sprites[CLOUD_3] = new Sprite("img/sprites/cloud-3.png");
    }
    else
    {
        this->sprites[CLOUD_1] = new Sprite("img/sprites/cloud-white-1.png");
        this->sprites[CLOUD_2] = new Sprite("img/sprites/cloud-white-2.png");
        this->sprites[CLOUD_3] = new Sprite("img/sprites/cloud-white-3.png");
    }

    this->usedClouds.resize(0);
    this->freeClouds.resize(this->maxAmmount);

    for (unsigned int i = 0; i < (this->maxAmmount); i++)
    {
        unsigned int index = SDL::randomNumberBetween(CLOUD_1, CLOUD_3);

        Cloud* tmp = new Cloud(0, 0, 0.2, this->sprites[index], i);
        this->freeClouds.push_back(tmp);
    }
}
CloudContainer::~CloudContainer()
{
    unsigned int size = this->sprites.size();
    for (unsigned int i = 0; i < CLOUD_MAX; i++)
        if (this->sprites[i])
            delete (this->sprites[i]);

    size = this->freeClouds.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (this->freeClouds[i])
            delete this->freeClouds[i];
    }

    size = this->usedClouds.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (this->usedClouds[i])
            delete this->usedClouds[i];
    }
}
void CloudContainer::addAt(Point p, float speed)
{
    if (this->freeClouds.size() == 0)
        return;

    if (this->currentAmmount >= this->maxAmmount)
        return;

    Cloud* tmp = this->freeClouds.back();

    Rectangle cloud(p.x,
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

    if (cloud.rightmost < this->areaLimit.leftmost  ||
        cloud.leftmost  > this->areaLimit.rightmost ||
        cloud.bottom    < this->areaLimit.top       ||
        cloud.top       > this->areaLimit.bottom)
        return;

    // "Creating" and adjusting
    tmp->setPosition(p);

    tmp->setSpeed(speed);
    tmp->setVisible(true);

    this->currentAmmount++;
    this->usedClouds.push_back(tmp);
    this->freeClouds.pop_back();

    Log::verbose("CloudContainer::addAt (" + SDL::intToString(p.x) +
                 ", " + SDL::intToString(p.y) +
                 ") " +
                 "Count: " + SDL::intToString(this->currentAmmount));
}
void CloudContainer::addAtRandom()
{
    // Cannot add anyway
    if (this->currentAmmount >= this->maxAmmount)
        return;

    // Will forcefully try to add until it works.
    // Watch out for those infinite loops!

    unsigned int previousAmmount = this->currentAmmount;
    int safetyCheckToAvoidInfiniteLoops;

    while (this->currentAmmount == previousAmmount)
    {
        safetyCheckToAvoidInfiniteLoops = 10;

        float speed = ((float)(SDL::randomNumberBetween(30, 70)));

        // Randomly deciding if the cloud's gonna go to the
        // left or right
        (SDL::randomBool())?
            speed *= -1:
            speed *=  1;

        float x = (this->areaLimit.leftmost + 1);

        // If the cloud's moving left then we'll add it on the
        // right limit.
        // If it's moving right, it'll be alright at the left
        // limit.
        if (speed < 0)
            x = (this->areaLimit.rightmost - 1);

        float y = SDL::randomNumberBetween(this->areaLimit.top,
                                           this->areaLimit.bottom);

        // This will fail silentrly if (x, y) is not inside bounds
        this->addAt(Point(x, y), speed);

        if (--safetyCheckToAvoidInfiniteLoops < 0)
            break;
    }
}
void CloudContainer::addAll()
{
    // Cannot add anyway
    if (this->currentAmmount >= this->maxAmmount)
        return;

    // Will forcefully try to add until it works.
    // Watch out for those infinite loops!
    int safetyCheckToAvoidInfiniteLoops;

    while (this->currentAmmount != this->maxAmmount)
    {
        safetyCheckToAvoidInfiniteLoops = 10;

        // This will fail silentrly if (x, y) is not inside bounds
        this->addAtRandom();

        if (--safetyCheckToAvoidInfiniteLoops < 0)
            break;
    }
}
void CloudContainer::update(float dt)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
    {
        if ((this->usedClouds[i]) &&
            (this->usedClouds[i]->isVisible()))
            this->usedClouds[i]->update(dt);

        // testing the limits
        Rectangle cloud(this->usedClouds[i]->getX(),
                        this->usedClouds[i]->getY(),
                        this->usedClouds[i]->getWidth(),
                        this->usedClouds[i]->getHeight());

        if (cloud.rightmost < this->areaLimit.leftmost  ||
            cloud.leftmost  > this->areaLimit.rightmost ||
            cloud.bottom    < this->areaLimit.top       ||
            cloud.top       > this->areaLimit.bottom)
            this->removeAt(i);

        // TODO
        // This has a MAJOR design flaw.
        // If I deleted a cloud at position i,
        // it will get swapped to the last and removed when I call removeAt(i).
        //
        // But since i is still an index to the array, it will
        // instantly point to the next one and it will skip.
    }
}
void CloudContainer::removeAt(unsigned int index)
{
    // Double-checking to make sure I won't remove invalid stuff
    if (this->currentAmmount == 0)
        return;

    unsigned int size = this->usedClouds.size();
    if (index >= size)
        return;

    Cloud* tmp = this->usedClouds[index];

    std::swap(this->usedClouds[index], this->usedClouds.back());

    Log::verbose("CloudContainer::removeAt (" + SDL::intToString(tmp->getX()) +
                 ", " + SDL::intToString(tmp->getY()) +
                 ") " +
                 "Count: " + SDL::intToString(this->currentAmmount - 1));

    this->freeClouds.push_back(tmp);
    this->usedClouds.pop_back();
    this->currentAmmount--;
}
void CloudContainer::render(float cameraX, float cameraY)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
    {
        if ((this->usedClouds[i]) &&
            (this->usedClouds[i]->isVisible()))
            this->usedClouds[i]->render(cameraX, cameraY);
    }
}
void CloudContainer::limitArea(Rectangle a)
{
    this->areaLimit = a;
}

