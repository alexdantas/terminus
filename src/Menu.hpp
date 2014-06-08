#ifndef MENU_H_DEFINED
#define MENU_H_DEFINED

#include <string>
#include <vector>
#include "Font.hpp"
#include "Text.hpp"
#include "SFX.hpp"

/// Container for individual menu item's stuff.
///
/// This represents each option of the menu.
///
/// ## How to use
///
/// You should never mess with this. The *Menu* class handles
/// it all internally.
///
struct MenuItem
{
    Text* text;  ///< Text that will be printed onscreen.
    int   value; ///< Item's custom value to identify itself.

    MenuItem(Text* text, int value);
};

/// General-purpose, customizable, easy-to-use in-game menu.
///
/// ## How to use
///
/// Let's say a game scene needs a menu. To use this class you
/// must do something like this:
///
///     Menu menu( /* initialization arguments */ );
///
///     menu.addItem("Start Game");
///     menu.addItem("Options");
///     menu.addItem("Exit");
///
/// Then, somewhere of your update code (game loop) do:
///
///     menu.update();
///
///     if (menu.optionWasSelected())
///     {
///         switch (menu.getSelectedOption())
///         {
///         // first option
///         case 0:
///             game.start();
///             break;
///
///         // second option
///         case 1:
///             options.doSomething();
///             break;
///
///             // and so on...
///         }
///     }
///
/// That handles both item-selection via keyboard and with mouse.
///
/// Notice that the options returned are in order of insertion. This
/// is very important. If you don't want to get weird bugs, you must
/// insert them at the right order (and nowhere else).
///
/// But there's another option. You could use custom values for each
/// item, saving you the touble of assuring the right order. Do the
/// following:
///
///     enum { MENU_START, MENU_OPTIONS, MENU_EXIT };
///
///     // Where you'd put initialization code
///     Menu menu( /* initialization arguments */ );
///
///     menu.addItem("Game Start", MENU_START);
///     menu.addItem("Options", MENU_OPTIONS);
///     menu.addItem("Exit", MENU_EXIT);
///
///     // Where you'd update your code
///     menu.update();
///
///     if (menu.optionWasSelected())
///     {
///         switch (menu.getSelectedOption())
///         {
///         case MENU_START:
///             game.start();
///             break;
///
///         case MENU_OPTIONS:
///             options.doSomething();
///             break;
///
///             // and so on...
///         }
///     }
///
/// Way easier, right? Either way, both methods are available, so
/// choose wisely.
///
/// // TODO Fix this
/// @bug If highlightFont size is greater than normalFont:
///      When we start the menu, by adding the first item,
///      we set it with the highlightFont.
///      So when we call centralizeText, it centralizes
///      all of the items, and the first, thinking that
///      the size of the first item is the size of it with
///      the highlightFont.
///      Finally, when we move the cursor, the first item
///      becomes non-centralized because the original
///      centralized position was calculated with the
///      other font.
///
/// // TODO Fix this
/// @bug isMouseInside() doesn't work when we consider the
///      cameraX and cameraY.
///      how can I do this?
///
class Menu
{
public:
    /// Creates a menu at *x*/*y*, rendered with *font* and
    /// highlighted with *hiliteFont*.
    Menu(Font* font, Font* hiliteFont, float x=0, float y=0);

    virtual ~Menu();

    /// Insers an item on the menu with *text* and optional
    /// custom *value*.
    ///
    /// The _value_ thing can be used to individually identify
    /// the current option on the menu.
    ///
    /// Also, you can work with the current option index. If you
    /// add items on an order, it's easy to guess what will be
    /// the first/second/whatever item - so no need for individual
    /// values.
    void addItem(std::string text, int value=-1);

    /// Returns the custom value of the current item.
    ///
    /// @return -1 if no items are set.
    int getCurrentItemValue();

    /// Returns the index of the currently selected item.
    ///
    /// @note Starts counting from zero!
    int getCurrentItemIndex();

    /// Returns how many items the menu has right now.
    int getAmmount();

    void next();     ///< Advances one item on the menu.
    void previous(); ///< Regresses one item on the menu.
    void first();    ///< Goes to the first item on the menu.
    void last();     ///< Goes to the last item on the menu.
    void random();   ///< Selects a random item from the menu.

    /// Shows the menu on screen.
    ///
    /// @note Make sure you call this on an update loop,
    ///       to render it correctly.
    void render(float cameraX=0, float cameraY=0);

    /// Updates it's internal state.
    ///
    /// @note Make sure you call this on an update loop,
    ///       to assure expected behaviour.
    void update(float dt=0);

    /// Centralizes the text of all the items.
    void centralizeText();

    bool optionWasSelected();
    int getSelectedOption();

private:

    Font* font;       ///< Font that will print the menu onscreen.
    Font* hiliteFont; ///< Font that prints the highlighted item.

    /// Container of all the options inside the menu.
    std::vector<MenuItem*> item;

    /// The ammount of items the menu has right now.
    int items;

    /// Index of the currently selected item.
    int currentItem;

    float x;    ///< Position on the *x* axis.
    float y;    ///< Position on the *y* axis.
    int width;  ///< Menu's full width in pixels.
    int height; ///< Menu's full height in pixels.

    /// Tells if the user selected an item right now.
    bool itemWasJustSelected;

    /// If an item was just selected, this points to it.
    MenuItem* selectedItem;

    float cameraX; ///< Stores the last-frame's camera x.
    float cameraY; ///< Stores the last0frame's camera y.

    SFX* selectionSFX;
    SFX* confirmSFX;
};

#endif //MENU_H_DEFINED

