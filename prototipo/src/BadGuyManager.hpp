#ifndef BADGUYMANAGER_H_DEFINED
#define BADGUYMANAGER_H_DEFINED

#include "BadGuyContainer.hpp"
#include "BadGuy.hpp"


class BadGuyManager
{
public:
    /// Will create *ammout* bad guys
    BadGuyManager(unsigned int ammout);
    virtual ~BadGuyManager();

    /// Show time
    void update(float dt);

    void render(float cameraX, float cameraY);

    /// Everyone
    BadGuyContainer* container;

    void add();

    void addAll();

    ///Get everyone
    std::vector<BadGuy*> getBadGuys();

private:

    /// Maximum distance on the Y axis between platforms.
    float maxHeight;

    float minHeight;

    BadGuy* badguy;

    /// Holds the camera Y to generate platforms.
    float currentCameraY;
};


#endif /* BADGUYMANAGER_H_DEFINED */
