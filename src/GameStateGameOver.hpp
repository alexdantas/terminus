#ifndef GAMESTATEGAMEOVER_H_DEFINED
#define GAMESTATEGAMEOVER_H_DEFINED

#include "GameState.hpp"
#include "Font.hpp"
#include "Text.hpp"
#include "Animation.hpp"
#include "Music.hpp"
#include "SFX.hpp"
#include "Fade.hpp"
#include "TimerCounter.hpp"

/// Shows that depressing screen when the player loses.
class GameStateGameOver: public GameState
{
public:
    GameStateGameOver();

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
    Font* font;
    Text* text1;
    Text* text2;
    Music* music;
    SFX *zeus;
    int stack;

     // Smooth effects to ease transitions.
    Fade* fadeWin;
    Fade* fadeLose;

    /// The image that will be shown on the intro.
    Sprite* win;
    Sprite* lose;
    Sprite* gameover;

    /// How long the image's allowed to stay on the screen.
    TimerCounter* imageTimer;
    TimerCounter* textTimer;

    int nextScene;
};

#endif //GAMESTATEGAMEOVER_H_DEFINED

