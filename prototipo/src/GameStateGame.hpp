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
#include "PlatformManager.hpp"
#include "CloudContainer.hpp"
#include "Fade.hpp"

/// This represents the actual game taking place.
///
/// Here is defined all the game logic and rules.
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

    static bool showBoundingBoxes;

private:

    // Such a mess, right?
	Sprite* bg;

	bool will_quit;
    bool will_return_to_main_menu;
    bool game_over;

    Camera* camera;

    Music* bgmusic;

    Player* apterus;

    ProgressBar* lifeBar;
    Font* lifeBarFont;
    Text* lifeBarText;

    // Internal state-specific functions

    /// Constructs the game map
    void buildMap();

	/// Creates a planet at a random position onscreen.
	void addPlanet();

	/// Destroys any planet that's dead.
	void checkPlanets();

	/// Controls events (primairly keyboard input).
	void processEvents();

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

    bool isPaused;

    Font* font;
    Text* pausedTitle;

    Font* consoleFont;
    DropDownConsole* console;

    PlatformManager* platforms;

    CloudContainer* cloudContainer;

    /// Fade-out effect that occurs when we're quitting the game.
    Fade* fadeOut;
};

#endif //GAMESTATEGAME_H_DEFINED

