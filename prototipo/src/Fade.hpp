#ifndef FADE_H_DEFINED
#define FADE_H_DEFINED

#include "SDL.hpp"
#include "Color.hpp"

/// Customizable effect of fading (screen fade-in/fade-out).
///
/// This allows smooth transition between screens.
///
/// The fade in paints the whole screen with the fade in color and
/// slowly becomes transparent.
///
/// The fade out starts transparent while slowly becoming visible,
/// finally painting the whole screen with the fade out color.
///
/// ## How to use:
///
///     Fade fadeInToBlack(Fade::FADE_IN, 0.3);
///     Fade fadeOutToWhite(Fade::FADE_OUT, 0.2, Color(255, 255, 255));
///
///     fadeInToBlack.start();
///
///     // Possibly inside a loop
///     {
///         if (fadeInToBlack.isDone())
///         {
///             fadeOutToBlack.stop();
///             fadeOutToWhite.start();
///         }
///     }
///
///     // Regardless if they've finished or not
///     fadeInToBlack.update(deltaBetweenFrames);
///     fadeOutToWhite.update(deltaBetweenFrames);
///
///     // Regardless if they've finished or not
///     fadeInToBlack.render();
///     fadeOutToWhite.render();
///
class Fade
{
public:
    /// Possible fades allowed.
    /// Is there any other possible type I'm missing here?
    enum FadeType { FADE_IN, FADE_OUT };

    /// Creates a fade to *color* with *speed* and *type*.
    ///
    /// @note By default fade-in/fade-out are from/to black.
    Fade(FadeType type, float speed, Color color=Color(0, 0, 0));

    virtual ~Fade();

    /// Starts the fading effect.
    ///
    /// @note If called again, restarts the fading.
    void start();

    /// Stops the fading effect.
    ///
    /// @note If called again, nothing happens.
    void stop();

    /// Updates the fading effect.
    ///
    /// @note Make sure to call this once per frame otherwise
    ///       the internal settings will be screwed.
    ///       Don't worry, if the fade is over, will return
    ///       immediately.
    void update(uint32_t dt);

    /// Renders the fade on the screen.
    ///
    /// @note Must be called every frame even if the fade
    ///       is over. It internally checks if it's over
    ///       and does the right thing.
    void render();

    /// Tells if the fade has finished.
    ///
    /// @note Don't use this to decide whether to call
    ///       *update()*/*render()*! They must be called
    ///       each frame.
    bool isDone();

private:
    FadeType type; ///< Current type of fade.

    float speed;   ///< Current fade speed.

    /// SDL's internal structure that represents the fade screen.
    SDL_Surface* surface;

    Color color;   ///< Color of the fade.

    bool done;     ///< Is the fade done?
    bool started;  ///< Has the fade started?
};

#endif //FADE_H_DEFINED

