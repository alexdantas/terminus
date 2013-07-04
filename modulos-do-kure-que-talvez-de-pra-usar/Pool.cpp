#include "Pool.hpp"

Pool::Pool()
{

}
Pool::~Pool()
{

}

T getNew()
{

}
void update(uint32_t dt)
{
    for (unsigned int i = 0; i < (this->currentAmmount); i++)
        if (this->usedItems[i])
            this->usedItems[i]->update(dt);
}
