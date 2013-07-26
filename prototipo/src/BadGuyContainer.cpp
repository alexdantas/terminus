#include "BadGuyContainer.hpp"

BadGuyContainer::BadGuyContainer(unsigned int maxAmmount, Rectangle *gameArea, PlatformManager *platforms) :
    maxAmmount(maxAmmount),
    currentAmmount(0),
    gameArea(gameArea),
    platforms(platforms)
 {
    unsigned int i;
    int occupied = 0; //Say's if platform already occupied by Venus copy

    for(i = 0; i < this->maxAmmount; i++)
    {
        Point p;

        if(i % 2 == 0)
            this->type = BadGuy::GRIFFIN;
        else
            this->type = BadGuy::VENUS;

        if(this->type == BadGuy::GRIFFIN)
        {
            p.x = rand()%this->gameArea->w;
            p.y = rand()%this->gameArea->h;

        }
        else
        {
            std::list<Platform*>::iterator it = this->platforms->container->platforms.begin();
            int j = 0;
            while(j != occupied && it != this->platforms->container->platforms.end())
            {
                it++;
                j++;
            }
            if(it != this->platforms->container->platforms.end() && (*it)->type != Platform::VANISHING && (*it)->type != Platform::MOVABLE)
                p = (*it)->box->topLeft;
            else
            {
                p.x = 0;
                p.y = 0;
            }
            occupied++;
        }
        if(p.x != 0 && p.y != 0)
            this->add(p, type);
    }
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

    if(type == BadGuy::VENUS){
        it = this->badguy.insert(it, new BadGuyVenus(p.x, p.y - 391, 142, 391, 1, Config::playerAcceleration));
        Log::verbose("Venus");
    }
    else{
        it = this->badguy.insert(it, new BadGuyGriffin(p.x - 292, p.y - 215, 292, 215, 1, Config::playerAcceleration));
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
            if(this->badguy[i]->died())
            {
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

