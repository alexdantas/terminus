#ifndef INPUTMANAGER_H_DEFINED
#define INPUTMANAGER_H_DEFINED

#include "SDL.hpp"

/// Handles input through keyboard and mouse.
//  It's the interface between SDL-direct input calls.
//
//  @note This class is a Singleton.
//  Tips on how to implement a Singleton:
//  http://stackoverflow.com/a/271104
//
//  @warning I TRIED IMPLEMENTING A SINGLETON BUT IT JUST KEEPS
//  GETTING ERRORS ON PRIVATE CONSTRUCTORS ARRGHHH
//
//  This class *needs* to be update()d every frame.
//
//  It collects all input events and saves on it's internal
//  data structures.
//  When requested, returns info about the current state - for
//  example, is a key currently pressed? Is where's the mouse?
//
/// ## How to use:
///
/// This class seems daunting at first but the interface is quite
/// simple.
///
/// TODO improve example
///
///     InputManager* input = InputManager::getInstance();
///
/// ## How to use (2):
///
/// There's another useful feature, "input lock/unlocking".
///
/// This is useful for any kind of situation on which you'd
/// like to prevent anything else from responding to input.
/// Like a cutscene, drop-down console, dialog...
///
/// For example:
///
///     void myFunction()
///     {
///         InputManager* input = InputManager::getInstance();
///         input.unlock();
///
///         // do some input stuff
///
///         input.lock();
///     }
///
/// The example above assures that the only place on the
/// program that responds to input will be inside *myFunction*.
///
/// There's no problem if you make multiple calls to
/// *lock()*/*unlock()*. The only thing you should make sure is
/// that for each *lock()* there should be a corresponding
/// *unlock()* - unless you purposely want to break your program.
///
/// Note that this does not prevent *InputManager* from reacting
/// to quit events. *quitRequested()* will function normally,
/// allowing you to quit the game even though the input is blocked.
///
class InputManager
{
public:
    /// Returns the singleton instance of this class.
    ///
    /// This assures that only a single instance of this class is
    /// created at any point on the game.
    static InputManager* getInstance();

    /// Updates the entire game input state.
    ///
    /// @note You should call this at every frame to assure
    ///        expected behaviour. Pretty much all things depend
    ///        on this.
    ///
    /// This method deals with very low-level SDL calls.
    ///
    /// But what it _really_ does is check the keyboard/mouse for
    /// any signs at all (keys down, keys up, mouse pressed,
    /// mouse X...) and update internal variables that reflect
    /// that.
    ///
    void update(float cameraX=0, float cameraY=0);

    /// Tells if the keyboard *key* was released just now.
    ///
    /// To see all possibilities, see isKeyPressed().
    bool isKeyDown(int key);

    /// Tells if the keyboard *key* was pressed just now.
    ///
    /// To see all possibilities, see isKeyPressed().
    bool isKeyUp(int key);

    /// Tells if the mouse *button* was released just now.
    ///
    /// To see all possibilities, see isMousePressed().
    bool isMouseDown(uint8_t button);

    /// Tells if the mouse *button* was pressed just now.
    ///
    /// To see all possibilities, see isMousePressed().
    bool isMouseUp(uint8_t button);

    /// Tells if the *key* is currently being pressed.
    ///
    /// To see all available keys, visit this site:
    /// http://wiki.libsdl.org/moin.fcg/SDL_Keycode
    bool isKeyPressed(int key);

    /// Tells if the mouse *button* is currently being pressed.
    ///
    /// Possible options are:
    ///     SDL_BUTTON_LEFT:   left mouse button
    ///     SDL_BUTTON_MIDDLE: middle mouse button
    ///     SDL_BUTTON_RIGHT:  right mouse button
    bool isMousePressed(uint8_t button);

    /// Tells if the user asked the game to quit.
    bool quitRequested();

    /// Returns the current mouse X position.
    int getMouseX();

    /// Returns the current mouse Y position.
    int getMouseY();

    /// Locks the *InputManager*, preventing it to actually
    /// refresh input and making all methods returning *false*
    /// (except for *quitRequested()*.
    ///
    /// For an explanation and tutorial, see the header of this
    /// class.
    void lock();

    /// Unlocks the *InputManager*, allowing it to respond
    /// normally to input.
    ///
    /// @see lock()
    void unlock();

    // Helper methods

    /// Tells if the mouse's currently inside the rectangle
    //  specified by #x, #y, #w and #h.
    bool isMouseInside(int x, int y, int w, int h);

    // Those deal with printable keys
    // (keys on the ASCII table that can be shown onscreen - not
    // control-characters)

    bool isPrintable(int key);
    bool isPrintableKeyDown();
    std::string getCurPrintableKey();

    // WUT

    InputManager();

    // Removing copy-constructor and assignment operator.
    InputManager(InputManager const&) {};
    void operator=(InputManager const&) {};

private:
    /// Current single instance of the class.
    static InputManager* instance;

    /// Saves SDL internal mouse state.
    uint8_t  mouse;

    /// Saves SDL internal keyboard state.
    uint8_t* keyboard;

    /// Current mouse X position.
    int  mouseX;

    /// Current mouse Y position.
    int  mouseY;

    /// Saves which keys are currently down.
    ///
    /// @note *KEYBOARD_SIZE* is defined on *SDL.hpp*.
    bool keyDown[KEYBOARD_SIZE];

    /// Saves which keys are currently up.
    ///
    /// @note *KEYBOARD_SIZE* is defined on *SDL.hpp*.
    bool keyUp[KEYBOARD_SIZE];

    /// Saves which mouse buttons are currently down.
    ///
    /// @note *MOUSE_SIZE* is defined on *SDL.hpp*.
    bool mouseDown[MOUSE_SIZE];

    /// Saves which mouse buttons are currently up.
    ///
    /// @note *MOUSE_SIZE* is defined on *SDL.hpp*.
    bool mouseUp[MOUSE_SIZE];

    /// Tells if we must quit the game.
    bool will_quit;

    /// If the user pressed a printable key, this is where
    /// it'll be stored.
    int curPrintableKey;

    /// Tells if the input manager is currently locked.
    bool isLocked;
};


#endif /* INPUTMANAGER_H_DEFINED */
