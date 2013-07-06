#ifndef GAMESTATEINTRO_H_DEFINED
#define GAMESTATEINTRO_H_DEFINED

#include "GameState.hpp"
#include "Sprite.hpp"
#include "Fade.hpp"
#include "TimerCounter.hpp"

/// Shows the introduction to the game.
///
/// Mainly shows a couple of images with fade in/out effect and
/// the jumps straight to the main menu.
///
class GameStateIntro: public GameState
{
public:
    GameStateIntro();

    /// Constructs everything necessary for the screen intro.
    void load(int stack=0);

    /// Destroys anything builded during the screen intro.
    int unload();

    /// Updates all possible things on the intro.
    ///
    /// @return A status code indicating what to do (what state
    ///         should we go to?). The codes are defined on
    ///         GameState.hpp.
    GameState::StateCode update(uint32_t dt);

    /// Shows everything onscreen;
    void render();

private:
    // Smooth effects to ease transitions.
    Fade* fadeIn;
    Fade* fadeOut;

    /// The image that will be shown on the intro.
    Sprite* image;

    /// How long the image's allowed to stay on the screen.
    TimerCounter* imageTimer;

    /// Counts the time before fade in starts (black screen).
    TimerCounter* fadeInTimer;
};

#endif //GAMESTATEINTRO_H_DEFINED

