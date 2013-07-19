#ifndef CLOUDMANAGER_H_DEFINED
#define CLOUDMANAGER_H_DEFINED

#include <vector>
#include "Sprite.hpp"
#include "Shapes.hpp"
#include "CloudContainer.hpp"

/// Includes and removes Clouds based on a set of rules.
///
/// ## For developers
///
/// This baby is very game-specific.
/// On this game, it creates clouds on a restricted *gameArea*,
/// removing any one that completely steps outside of it.
/// You are free to update the *gameArea* as you wish.
///
class CloudManager
{
public:
    /// Will create at most *maxAmmount* clouds at the same
    /// time inside *gameArea*.
    CloudManager(Rectangle gameArea, int maxAmmount);
    virtual ~CloudManager();

    /// Sets the time to wait before adding clouds (in ms).
    ///
    /// ## Example
    ///     clouds.setDelay(1000); // 1 second
    void setDelay(int delay);

    /// Limits the area on which Clouds can exist to *area*.c
    void setArea(Rectangle area);

    /// Updates all clouds and cloud handling scheme.
    void update(float dt);

    /// Renders all clouds.
    void render(float cameraX, float cameraY);

    /// Forces a new Cloud on the screen.
    void add();

    /// Forces insertion of all possible Clouds.
    void addAll();

private:
    /// All the clouds of the game.
    CloudContainer* container;

    /// The maximum allowed size on which the clouds are generated.
    Rectangle gameArea;

    /// Controls the time-counting scheme to add clouds.
    TimerCounter timer;
};

#endif //CLOUDMANAGER_H_DEFINED

