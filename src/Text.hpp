#ifndef TEXT_H_DEFINED
#define TEXT_H_DEFINED

#include <string>
#include "Font.hpp"
#include "SDL.hpp"

/// Text to be printed on the screen with a particular *Font*.
///
/// @see Font.hpp
///
/// ## How to use:
///
/// This class allows text to be rendered onscreen with various attributes.
/// But it depends a lot on what font you render it with.
///
///     Font font("ttf/Terminus.ttf", 24);
///     Text text(&font);
///
///     text.setText("This is some sample text");
///     text.setPosition(20, 23);
///
///     // On a loop somwhere, just to make sure it will get
///     // shown every frame:
///     text.render();
///
///     // Or if you want to take a camera into consideration:
///     text.render(20, 12);
///     // This will make sure to print the text on the right
///     // position, as if the camera was located at (20, 12).
///
class Text
{
public:
    /// Possible ways the text can be aligned.
    enum Alignment { CENTER, LEFT, RIGHT };

    /// Creates a text with *font*.
    Text(Font* font=NULL);

    virtual ~Text();

    /// Sets the current text to *text*.
    void setText(std::string text="");

    /// Sets the place where we'll print the text;
    void setPosition(float x=0, float y=0);

    /// Sets how the text will be aligned on the screen.
    ///
    /// @bug Some alignments are not implemented. Actually,
    ///      the only one is CENTER. But by default the text
    ///      stays aligned LEFT.
    void setAlignment(Alignment align);

    /// Changes the text's font.
    void setFont(Font* font);

    /// Refreshes internal data structures so the text can be
    /// printed on the screen.
    ///
    /// @note Must be called if the text is changed.
    void refresh();

    /// Shows the text on screen.
    void render(float cameraX=0, float cameraY=0);

    float getX();
    float getY();

    /// Returns the actual pixel width of the text box;
    int getWidth();

    /// Returns the actual pixel height of the text box;
    int getHeight();

    /// Returns the number of lines of the text.
    int getLines();

    /// Returns the number of columns of the text.
    ///
    /// @note If it has several lines, returns the length of
    /// the biggest one.
    int getCols();

private:
    /// The font that will be used to show the text.
    Font* font;

    /// SDL's internal data structure holding the text image.
    SDL_Surface* surface;

    /// The actual text that's going to be displayed.
    std::string text;

    float x; ///< Text's x position.
    float y; ///< Text's y position.

    /// Current text alignment.
    Alignment align;
};

#endif //TEXT_H_DEFINED

