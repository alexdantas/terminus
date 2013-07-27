#ifndef GAMESTATEMAINMENU_H_DEFINED
#define GAMESTATEMAINMENU_H_DEFINED

#include "GameState.hpp"
#include "Font.hpp"
#include "Music.hpp"
#include "Text.hpp"
#include "Menu.hpp"
#include "Music.hpp"
#include "Sprite.hpp"
#include "CloudContainer.hpp"
#include "Fade.hpp"

/// Everything that happens on the main menu.
class GameStateMainMenu: public GameState
{
public:
    GameStateMainMenu();
    virtual ~GameStateMainMenu();

    /// Constructs everything necessary for the menu.
    void load(int stack=0);

    /// Destroys anything builded during the menu.
    int unload();

    /// Updates all possible things on the menu.
    ///
    /// @return A status code indicating what t odo (should we
    ///         change states/quit the game?). The codes are
    ///         defined on GameState.hpp.
    GameState::StateCode update(float dt);

    /// Shows everything onscreen;
    void render();

private:
    Font* font;       ///< How the menu will show onscreen.
    Font* hiliteFont; ///< How menu's current item will be shown.
    Music* music;     ///< Menu's background music.

    Menu* menu;       ///< The actual game menu.

    Music* bgmusic;

    Sprite* bg;       ///< The background image.
    Sprite* logo;     ///< The logo image.
    Sprite* detail;   ///< Some nice details.

    CloudContainer* clouds;

    Fade* fade; ///< The fade effect of the screen.
    Fade* fadeOut;
};

#endif //GAMESTATEMAINMENU_H_DEFINED

