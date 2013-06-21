#ifndef DROPDOWNCONSOLE_H_DEFINED
#define DROPDOWNCONSOLE_H_DEFINED

#include <vector>
#include "Window.hpp"
#include "Font.hpp"
#include "SDL.hpp"
#include "Color.hpp"
#include "Timer.hpp"

/// Represents a custom command.
///
/// ## How to use it
///
/// When you're using the *DropDownConsole*, you usually type commands
/// to it. Well, how do you assure it does what you want it to?
///
/// Simple, just assign a command to the *DropDownConsole*! For
/// instance, say you want to kill the main character whenever the
/// user sends the command `kill`.
///
/// Somewhere on your initialization code, put those calls:
///
///     DropDownConsole console( /* initialization stuff */);
///
///     console.addCommand("kill", 0);
///
/// So, whenever the user sends `kill`, two things will happen:
///
/// 1.*hasCommand()* will return *true*.
/// 2.*getCommand()* will return the value you just told right
///   above (in this case, 0).
///
/// On some part of your update code (game loop), check it with:
///
///     if (console.hasCommand())
///     {
///         int command = console.getCommand();
///         if (command == 0)
///             mainCharacter.kill();
///     }
///
/// Get it? I suggest you use an *enum* for all possible commands.
/// Just like:
///
///     enum Commands { KILL, QUIT, MAKE_COFFEE };
///
///     // ..somewhere buried deep inside your code..
///
///     if (console.hasCommand())
///     {
///         switch (console.getCommand())
///         {
///         case KILL:
///             mainCharacter.kill();
///             break;
///         case QUIT:
///             game.quit();
///             break;
///         // ...
///     }
///
/// And so on.
///
struct Command
{
    std::string command; ///< String of the command
    int value;           ///< Programmer-assigned value to the command

    Command(std::string command, int value);
};

/// General history feature.
///
/// Contains a history of strings. Can be both commands and
/// general output.
struct DropDownConsoleHistory
{
    int maxSize;
    int currentIndex;
    int firstIndex;
    int lastIndex;

    std::vector<std::string> history;

    DropDownConsoleHistory(int maxSize);
    void add(std::string text);
    int size();
    std::string get(int index);
    std::string getFirst();
    std::string getLast();
    std::string getNext();
    std::string getPrevious();
    std::string getCurrent();
    void clear();

    void setMaxSize(int value);
};

/// A terminal-like textual interface, with drop-down animation,
/// just like the Quake console.
///
/// It's very flexible, being highly customizable and easy-to-use.
///
/// For more info on how to get/set commands, see the *Command* struct.
/// @see Command
///
///
class DropDownConsole
{
public:
    /// All possible console states (how it's shown onscreen.
    enum ConsoleState { INVISIBLE, SCROLLING_DOWN, SCROLLING_UP, VISIBLE };

    DropDownConsole(Font* font, int x=0, int y=0, int w=Window::width, int h=Window::height/2);
    virtual ~DropDownConsole();

    /// Inserts a possible command to the console.
    ///
    /// For more info (and a tutorial), see the *Command* struct.
    /// @see Command
    void addCommand(std::string command, int value);

    /// Tells if the user has just sent a command to the console.
    ///
    /// For more info (and a tutorial), see the *Command* struct.
    /// @see Command
    bool hasCommand();

    /// Returns the most-recently sent command by the user.
    ///
    /// For more info (and a tutorial), see the *Command* struct.
    /// @see Command
    int  getCommand();

    /// Sets the key that brings the console down/up.
    void setDropDownKeybinding(int key=SDLK_F12);

    /// Sets the background color of the console.
    ///
    /// @note It supports the Alpha channel. In other words, you
    ///       can make it transparent by sending a color with
    ///       alpha settings.
    void setBackgroundColor(Color color=Color(0, 0, 0));

    /// Sets the foreground color of the text on the console.
    void setForegroundColor(Color color=Color(255, 255, 255));

    /// Sets the foreground color of the text of the prompt.
    ///
    /// @see setPrompt()
    void setPromptForegroundColor(Color color=Color(255, 255, 255));

    /// Sets the drop-down speed of the console.
    void setScrollSpeed(float speed);

    /// Sets the promp of the console (string that appears before
    /// any command).
    void setPrompt(std::string prompt);

    /// Refresh the console's internal state and appearance.
    ///
    /// @note Must be called after any appearance modifications (like
    ///        background/foreground sets).
    ///
    /// @note It ALSO must be called before the first screen render!
    void refresh();

    /// Shows console, taking in consideration the camera's positions.
    ///
    /// @note You must put this method on a rendering loop. Don't worry,
    ///       it will only show the console when it's visible.
    void render(float cameraX=0, float cameraY=0);

    /// Updates the console's internal state.
    ///
    /// @note You must call this every frame, to assure the console's
    ///       correct funcion!
    void update(uint32_t dt);

    /// Tells if the console's visible (if it hasn't been dropped down).
    bool isVisible();

    /// Sends a string to the console output.
    void print(std::string text);

    /// Inserts the command *string* on the command buffer.
    void printCommand(std::string string);

    /// Clears the console's output history.
    void clearHistory();

private:


    float x;    ///< The console's position on the x axis.
    float y;    ///< The console's position on the y axis.
    int width;  ///< The console's width.
    int height; ///< The console's height.

    float scrollSpeed; ///< How fast the console scrolls when dropped down.

    ConsoleState state; ///< Current state (visible/invisible/scrolling).

    Font* font; ///< The font that renders all the text.

    SDL_Surface* surface; ///< SDL's internal data structure.

    Color background;
    Color foreground;
    Color promptForeground;

    int horizontalChars; ///< Number of visible chars horizontally.
    int verticalChars;   ///< Number of visible chars vertically.

    /// Key that calls the dropdown function.
    ///
    /// By default, *F12*.
    ///
    /// @note Must be recognizable by the InputManager.
    /// @see InputManager
    int keybinding;

    /// How much scrolled the console is.
    ///
    /// The bigger this is, the farthest the console is from the top.
    /// @note The maximum size is, of course, the console height.
    ///       Also, if it's zero, the console is full hidden.
    int topOffset;

    /// String that waits for the user to press something.
    std::string prompt;

    /// Stores the current user input, right before he presses *enter*.
    std::string inputBuffer;

    /// Stores a history of user's commands.
    std::vector<std::string> history;

    DropDownConsoleHistory outputHistory;
    DropDownConsoleHistory commandHistory;

    int curHistorySize;
    int maxHistorySize;

    int cursor; ///< The cursor position on the *inputBuffer*.

    std::vector<Command*> commands; ///< Has all the possible commands.

    Command* currentCommand; ///< Tells the command the user just sent.

    bool userJustSentACommand; ///< Tells if the user just sent a command.

    // Somewhat hidden internal functions.
    // (mostly helpers to avoid code cluttering)
    //
    // Don't call these unless you know what you're doing.
    // They're already taken care of internally.

    void updateAnimation(uint32_t dt); ///< Refresh animation, if necessary.
    void updateInput(); ///< Refresh input events.
    void renderLines(); ///< Shows all lines onscreen.

    /// Inserts *string* on the *inputBuffer* at the cursor position.
    void addStringOnCursor(std::string string);

    void cursorMoveLeft();
    void cursorMoveRight();
    void cursorHome();
    void cursorEnd();
    void cursorDelete();
    void cursorBackspace();
    void cursorClear();
};

#endif //DROPDOWNCONSOLE_H_DEFINED

