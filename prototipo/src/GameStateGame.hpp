#ifndef GAMESTATEGAME_H_DEFINED
#define GAMESTATEGAME_H_DEFINED

// Gaaah, #include hell!
#include <vector>
#include <cstdlib>
#include "GameState.hpp"
#include "SDL.hpp"
#include "Sprite.hpp"
#include "InputManager.hpp"
#include "Camera.hpp"
#include "Music.hpp"
#include "Animation.hpp"
#include "ProgressBar.hpp"
#include "Font.hpp"
#include "Text.hpp"
#include "Menu.hpp"
#include "DropDownConsole.hpp"
#include "Player.hpp"
#include "BadGuyManager.hpp"
#include "PlatformManager.hpp"
#include "CloudContainer.hpp"
#include "Fade.hpp"
#include "TimerCounter.hpp"

#include "Log.hpp"

/// This represents the actual game taking place.
///
/// Here is defined all the game logic and rules.
///
/// ## For developers:
///
/// Before you continue, take a deep breath.
///
/// Done? OK.
/// This is the most complex class of this game and it uses
/// pretty much every other class on the project.
///
/// To understand it you must have a lot of patience and that
/// "I'll see what it means later" feeling.
///
/// Don't try to understand everything at once!
/// Try to abstract a litte and follow the code thinking only in
/// terms of what you think it's doing.
///
/// After you've quite guessed what a method does, start looking
/// into the classes that it uses.
///
class GameStateGame: public GameState
{
public:
    GameStateGame();
    virtual ~GameStateGame();

    /// Constructs everything necessary for the game.
    void load(int stack=0);

    /// Destroys anything builded during the game
    int unload();

    /// Updates all possible things on the game.
    ///
    /// @return A status code indicating what to do (should we
    ///         change states/quit the game?). The codes are
    ///         defined on GameState.hpp.
    GameState::StateCode update(float dt);

    /// Shows everything onscreen;
    void render();

private:

    // Internal state-specific methods.
    //
    // They're here to ease reading -  each one of them gets
    // called once per frame.

	/// Receives input and tells what to do with it.
	///
	/// @note Every class that needs input deals with it internally.
	///       For example, the Player's input-handling method is
	///       inside the Player class!
	///       The input here is specific to the Game State.
	void updateInput();

    /// Checks and resolves all possible collisions during this state.
    ///
    /// ## For developers
    ///
    /// Instead of each object checking it's collision with everyone,
    /// I've decided to make each state check AND resolve collisions
    /// for every object it contains.
    ///
    /// It makes more sense since each state needs it's own collision
    /// rules and knows which objects to check collision for.
    /// (For example, at the main menu I might have platforms there
    ///  but wouldn't want to check collisions for them)
    ///
    /// This way, every object knows how to update and move itself
    /// but the state ALLOWS it or not.
    ///
    /// If the state doesn't allow the object to move, then it will
    /// be stuck, just like when you can't pass through the ground
    /// because the "Physics Rules of the World" don't allow you to
    /// (even though the gravity keeps pulling you every second).
    ///
    void checkCollisions();

    /// Checks and resolves anything related to platforms.
    ///
    /// For example, if anything's above a moving platform, we
    /// will move it here.
    void checkPlatforms();

    // Now, the variables.

	Sprite* bg; ///< Background of the game.

	bool will_quit; ///< If we're gonna quit the game.
    bool will_return_to_main_menu; ///< If we're gonna go to main menu.
    bool game_over; ///< If the game's over.

    Camera* camera; ///< The camera that shows part of the whole game screen.

    TimerCounter *timer; ///< For a not immediately game over

    Music* bgmusic;

    Player* apterus; ///< The player - main controllable class of the game.

    ProgressBar* lifeBar;
    Font* lifeBarFont;
    Text* lifeBarText;

    bool isPaused; ///< Is the game paused?

    Font* font; ///< Font that shows everything on the screen.
    Text* pausedTitle;

    Font* consoleFont;        ///< The game console font.
    DropDownConsole* console; /// The game console.

    PlatformManager* platforms; ///< All the game platforms.

    BadGuyManager* badguy; ///All the enemys in the game

    CloudContainer* cloudContainer; ///< All the clouds of the game.

    /// Fade-out effect that occurs when we're quitting the game.
    Fade* fadeOut;
};

#endif //GAMESTATEGAME_H_DEFINED

