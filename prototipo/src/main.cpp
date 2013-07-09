#include <exception>
#include "StateManager.hpp"
#include "Log.hpp"

/// Macro to avoid compiler warnings on unused variables.
#define UNUSED(x) ((void)(x))

/// Starting point of the game.
///
/// Thank you for having the trouble of looking out my code!
///
/// # Note for developers
///
/// Sorry for disappointing you.
/// This function's so small because everything is handled via the
/// *StateManager* class. It decides what to do based on the
/// current game state (for example the main menu, or game over
/// screen).
///
/// If you want to understand the code, I suggest you to read the
/// *StateManager* first - just to get an idea on how the states are
/// switched.
///
/// Then, you may see the individual state files, on the order
/// of appearance (you can check that on *StateManager::run()*.
///
int main(int argc, char* argv[])
{
    // Shut up, damn compiler!
    UNUSED(argc);
    UNUSED(argv);

    try
    {
        // Creating the screen (width, height) and states.
        StateManager manager(800, 600);

        manager.run();
    }
    catch (...)
    {
        // I dont really have a nice exception-handling scheme right
        // now. I must learn how to properly deal with them.
        // As of now, everything that fucks up gets the program
        // terminated.
        Log::error("Exception caught! Quitting...");
        return -1;
    }

    return 0;
}

