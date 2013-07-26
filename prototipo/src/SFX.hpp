#ifndef SFX_H_DEFINED
#define SFX_H_DEFINED

#include "SDL.hpp"

/// Sound effects for the win!
//
//  You can load and play a SFX.
//
class SFX
{
public:
    /// Initializes pretty much everything.
    //  Yeah, don't bother calling any of the functions below unless
    //  you want to override the SFX filepath.
    SFX(std::string filepath);

    virtual ~SFX();

    /// Plays the loaded SFX.
    //
    //  @param times How many times we'll play the SFX. If -1,
    //  infinite loop.
    //
    //  @note If called multiple times, will wait for it to
    //        finish before playing again.
    bool play(int times=-1);

    /// Loads the SFX into memory.
    //
    //  This is automatically called when initialized, don't bother
    //  with it.
    bool load();

    /// Sets the current SFX to be the one on #filepath.
    void setSFX(std::string filepath);

    /// This function's called automatically when the SFX stops.
    //  It warns internally that the SFX has stopped.
    //  @note Don't you dare call it! False alarms are not welcome.
    //
    //  BUG: Mix doesn't allow you to define a callback liek this.
    //       what should I do?
    //void finishedPlaying();

private:
    Mix_Chunk*  sfx;
    std::string filepath;
};

#endif /* SFX_H_DEFINED */
