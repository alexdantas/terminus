#ifndef BADGUYGGRIFFIN_H_DEFINED
#define BADGUYGGRIFFIN_H_DEFINED

#include "BadGuy.hpp"
#include "TimerCounter.hpp"

/**
* Griffin is a really, really bad guy
*
* In Griffin class, the only changes are:
* - Sprite sheet (duh)
* - The way he moves~~
*
* More details in Bad Guy Container class ;
*/
class BadGuyGriffin : public BadGuy
{
public:
    BadGuyGriffin(float x, float y, int w, int h, int hp, float acceleration);
    virtual ~BadGuyGriffin();
    void update(float dt); //Thw way Griffins moves
private:
    TimerCounter *timer;

    bool goDown; //if griffin now haves to go down and attacks

};

#endif //BADGUYGRIFFIN_H_DEFINED
