#ifndef GAMESTATEINSCTRUCTION_H_DEFINED
#define GAMESTATEINSCTRUCTION_H_DEFINED

#include "GameState.hpp"
#include "Font.hpp"
#include "Text.hpp"
#include "Animation.hpp"
#include "Music.hpp"
#include "SFX.hpp"
#include "Fade.hpp"
#include "TimerCounter.hpp"

/// Shows that depressing screen when the player loses.
class GameStateInstruction: public GameState
{
public:
    GameStateInstruction();

    /// Constructs everything necessary for the screen.
    void load(int stack=0);

    /// Destroys anything builded during the screen.
    int unload();

    /// Updates all possible things on the screen.
    ///
    /// @return A status code indicating what to do (should we
    ///         change states/quit the game?). The codes are
    ///         defined on GameState.hpp.
    GameState::StateCode update(float dt);

    /// Shows everything onscreen;
    void render();

private:

    /// The image that will be shown on the intro.
    Sprite* image;

};

#endif //GAMESTATEINSCTRUCTION_H_DEFINED

