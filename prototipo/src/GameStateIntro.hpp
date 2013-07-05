#ifndef GAMESTATEINTRO_H_DEFINED
#define GAMESTATEINTRO_H_DEFINED

#include "GameState.hpp"
#include "Sprite.hpp"
#include "Fade.hpp"
#include "Timer.hpp"

/// Shows the introduction to the game.
class GameStateIntro: public GameState
{
public:
    GameStateIntro();

    /// Constructs everything necessary for the screen.
    void load(int stack=0);

    /// Destroys anything builded during the screen.
    int unload();

    /// Updates all possible things on the screen.
    ///
    /// @return A status code indicating what to do (should we
    ///         change states/quit the game?). The codes are
    ///         defined on GameState.hpp.
    int update(uint32_t dt);

    /// Shows everything onscreen;
    void render();

private:
    Fade* fadeIn;
    Fade* fadeOut;

    Sprite* image;

    Timer* timer;
};

#endif //GAMESTATEINTRO_H_DEFINED

