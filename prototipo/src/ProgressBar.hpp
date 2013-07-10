#ifndef PROGRESSBAR_H_DEFINED
#define PROGRESSBAR_H_DEFINED

#include "SDL.hpp"
#include "Color.hpp"

/// Defines a general-purpose progress bar.
///
class ProgressBar
{
public:
    /// Creates a progress bar with *width* and *height* (both
    /// in pixels as how will be shown onscreen), and *max* and
    /// *starting* values of the progress.
    ///
    /// It grows from the left to the right.
    ProgressBar(float x, float y, int width, int height, int max=100, int starting=0);

    virtual ~ProgressBar();

    /// Increases progress by *ammount*.
    void increase(int ammount);

    /// Decreases progress by *ammount*.
    void decrease(int ammount);

    /// Set progress by *ammount*.
    void set(int ammount);

    /// Tells if the progress bar is currently full.
    bool isFull();

    /// Tells if the progress bar is still at 0%.
    bool isEmpty();

    /// Color of the empty parts.
    void setBackgroundColor(Color color=Color(0, 0, 0));

    /// Color of the filled parts.
    void setForegroundColor(Color color=Color(255, 255, 255));

    /// Shows progress bar based on *cameraX* and *cameraY*.
    void render(float cameraX=0, float cameraY=0);

private:
    float x; ///< X position on the screen.
    float y; ///< Y position on the screen.

    int maxValue;
    int currentValue;
    int width;
    int height;

    bool isComplete; ///< Is the bar complete.
    bool isZero;     ///< Is the bar empty.

    SDL_Surface* surface; ///< SDL's internal data structure.

    Color filledColor; ///< Color of the filled part.
    Color emptyColor;  ///< Color of the empty part.
};

#endif //PROGRESSBAR_H_DEFINED

