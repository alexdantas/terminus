#include "BadGuyContainer.hpp"
#include "SDL.hpp" // randomNumberBetween()
#include "Window.hpp"


BadGuyContainer::BadGuyContainer(unsigned int maxAmmount, Rectangle *gameArea, PlatformManager *platforms) :
    venusCount(0),
    griffinCount(0),
    maxAmmount(maxAmmount),
    currentAmmount(0),
    gameArea(gameArea),
    platforms(platforms)
{
    unsigned int i;
    int occupied = 0; // Say's if platform already occupied by Venus copy

    this->addGriffin();
    this->addGriffin();
    this->addGriffin();
    this->addGriffin();
    this->addGriffin();
    this->addGriffin();
    this->addGriffin();
    this->addGriffin();
    this->addGriffin();
    this->addGriffin();
    // ADDING EVERYONE
    // for (i = 0; i < this->maxAmmount; i++)
    // {
    //     Point p;

    //     if ((i % 2) == 0)
    //         this->type = BadGuy::GRIFFIN;
    //     else
    //         this->type = BadGuy::VENUS;

    //     if (this->type == BadGuy::GRIFFIN)
    //     {
    //         p.x = SDL::randomNumberBetween(0, this->gameArea->w);
    //         p.y = SDL::randomNumberBetween(0, this->gameArea->h);
    //     }
    //     else // VENUS
    //     {
    //         std::list<Platform*>::iterator it = this->platforms->container->platforms.begin();
    //         int j = 0;
    //         while (j != occupied && it != this->platforms->container->platforms.end())
    //         {
    //             it++;
    //             j++;
    //         }

    //         if (it != this->platforms->container->platforms.end() &&
    //             (*it)->type != Platform::VANISHING                &&
    //             (*it)->type != Platform::MOVABLE)
    //         {
    //             p = (*it)->box->topLeft;
    //         }
    //         else
    //         {
    //             p.x = 0;
    //             p.y = 0;
    //         }
    //         occupied++;
    //     }
    //     if (p.x != 0 && p.y != 0)
    //         this->add(p, type);
    // }
 }
BadGuyContainer::~BadGuyContainer()
{
    unsigned int size = this->badguy.size();
    for (unsigned int i = 0; i < size; i++)
        if (this->badguy[i])
            delete (this->badguy[i]);
}
void BadGuyContainer::add(Point p, BadGuy::BadGuyType type)
{
    std::vector<BadGuy*>::iterator it;
    it = this->badguy.begin();

    if (type == BadGuy::VENUS)
    {
        BadGuyVenus* v = new BadGuyVenus(p.x, p.y - 391, 142, 391, 1, Config::playerAcceleration);
        it = this->badguy.insert(it, v);

        this->venus.push_back(v);
        Log::verbose("Venus");
    }
    else
    {
        BadGuyGriffin* g = new BadGuyGriffin(p.x - 292, p.y - 215, 292, 215, 1, Config::playerAcceleration);
        it = this->badguy.insert(it, g);

        this->griffin.push_back(g);
        Log::verbose("Griffin");
        (*it)->setHorizontalLimit(60, this->gameArea->w - 119);
        (*it)->setVerticalLimit(215, rand()%this->gameArea->h);
    }

    Log::verbose("BadGuy::add (" + SDL::intToString(p.x) +
                 ", " + SDL::intToString(p.y) +
                 ") " );
}
void BadGuyContainer::render(float cameraX, float cameraY)
{
    this->cameraY = cameraY;

    unsigned int size = this->badguy.size();
    for (unsigned int i = 0; i < size; i++)
        if (this->badguy[i])
            this->badguy[i]->render(cameraX, cameraY);
}
void BadGuyContainer::update(float dt)
{
    unsigned int size = this->badguy.size();
    for (unsigned int i = 0; i < size; i++)
        if (this->badguy[i])
        {
            // Will kill it if it's below current camera level
            if ((this->badguy[i]->box->bottom) > (this->cameraY + Window::height + 100))
                this->badguy[i]->die();

            if (this->badguy[i]->died())
            {
                if (this->badguy[i]->type == BadGuy::VENUS)
                    this->venusCount--;
                else
                    this->griffinCount--;

                delete this->badguy[i];
                this->badguy[i] = NULL;
            }
            else
            {
                this->badguy[i]->update(dt);
                this->badguy[i]->commitMovement();
            }
        }
}
void BadGuyContainer::addVenus()
{
    for (std::list<Platform*>::iterator it = this->platforms->container->platforms.begin();
         it != this->platforms->container->platforms.end();
         it++)
    {
        // Kinda limited, right?
        if (((*it)->type == Platform::VANISHING) ||
            ((*it)->type == Platform::MOVABLE)   ||
            ((*it)->type == Platform::CLOUD)     ||
            ((*it)->occupied))
            continue;

        // Adding only on platforms above the current camera level
        if ((*it)->box->bottom < this->cameraY)
        {
            this->addVenusTo((*it));
            break;
        }
    }
}
void BadGuyContainer::addVenusTo(Platform* p)
{
    p->occupied = true;

    BadGuyVenus* v = new BadGuyVenus(p->box->center.x - 30,
                                     p->box->top - 270,
                                     98,
                                     270,
                                     1,
                                     Config::playerAcceleration);
    this->badguy.push_back(v);
    this->venusCount++;

    Log::verbose("Venus");
    Log::verbose("BadGuy::add (" + SDL::intToString(p->box->x) +
                 ", " + SDL::intToString(p->box->y) +
                 ") " );
}
void BadGuyContainer::addGriffin()
{
    Point p;
    p.x = SDL::randomNumberBetween(0, this->gameArea->w);
    p.y = SDL::randomNumberBetween(0, this->gameArea->h);

    BadGuyGriffin* g = new BadGuyGriffin(p.x - 292,
                                         p.y - 215,
                                         292,
                                         215,
                                         1,
                                         Config::playerAcceleration);

    g->setHorizontalLimit(60, this->gameArea->w - 119);
    g->setVerticalLimit(215, SDL::randomNumberBetween(0, this->gameArea->h));

    this->badguy.push_back(g);
    this->griffinCount++;

    Log::verbose("Griffin");
    Log::verbose("BadGuy::add (" + SDL::intToString(p.x) +
                 ", " + SDL::intToString(p.y) +
                 ") " );
}

