#ifndef BADGUYMANAGER_H_DEFINED
#define BADGUYMANAGER_H_DEFINED

#include "BadGuyContainer.hpp"
#include "BadGuy.hpp"


class BadGuyManager
{
public:
    /// Will create *ammout* bad guys
    BadGuyManager(unsigned int ammout, Rectangle *gameArea, PlatformManager *platforms);
    virtual ~BadGuyManager();

    /// Show time
    void update(float dt);

    void render(float cameraX, float cameraY);

    /// Everyone
    BadGuyContainer* container;

    ///Get everyone
    std::vector<BadGuy*> getBadGuys();

    BadGuy* badguy;

private:

    /// Maximum distance on the Y axis between platforms.
    float maxHeight;

    float minHeight;

    /// Holds the camera Y to generate platforms.
    float currentCameraY;
};


#endif /* BADGUYMANAGER_H_DEFINED */
