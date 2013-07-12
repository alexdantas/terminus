#ifndef BADGUYCONTAINER_H_DEFINED
#define BADGUYCONTAINER_H_DEFINED

#include "BadGuy.hpp"
#include "Log.hpp"
#include "BadGuyGriffin.hpp"
#include "BadGuyVenus.hpp"
#include "PlatformManager.hpp"
#include "Config.hpp"
#include <vector>

/// Contains all bad guys in the game
///
/// A set of bad guys to be used.
///
class BadGuyContainer
{
public:
    /// Creates a bad guy container that holds *maxAmmount* of
    /// bad guys
    BadGuyContainer(unsigned int maxAmmount);

    virtual ~BadGuyContainer();

    /// Adds a bad guy with *type* on point *p*.
    void add(Point p, BadGuy::BadGuyType type);

    /// Show time
    void render(float cameraX, float cameraY);

    void update(float dt);

    /// The whole raw list of bad guys.
    //In a vector, which is the ~~~~~same~~~~~~ as list
    //But easier to manipulate, SORRY!!!!!
    std::vector<BadGuy*>badguy;


private:
    /// Maximum ammount of bad guys allowed.
    unsigned int maxAmmount;

    /// Current bad guy.
    unsigned int currentAmmount;

    /// Contains indexes to active objects.
    std::vector<int> indirection;
};

#endif //BADGUYCONTAINER_H_DEFINED
