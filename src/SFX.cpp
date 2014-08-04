#include "SFX.hpp"
#include "Log.hpp"

/* returns the duration of a `Mix_Chunk` in milliseconds
 * Source: http://osdl.sourceforge.net/OSDL-0.4/api/html/OSDLAudio_8cc-source.html
 *
 * Untested code follows...
 */
Uint32 getChunkTimeMilliseconds(Mix_Chunk *chunk)
{
	Uint32 points = 0;
	Uint32 frames = 0;
	int freq      = 0;
	Uint16 fmt    = 0;
	int chans     = 0;

	// Chunks are converted to audio device format...
	if (!Mix_QuerySpec(&freq, &fmt, &chans))
		return 0;  //never called Mix_OpenAudio()?!

	// bytes / samplesize == sample points
	points = (chunk->alen / ((fmt & 0xFF) / 8));

	// sample points / channels == sample frames
	frames = (points / chans);

	// (sample frames * 1000) / frequency == play length in ms
	return (frames * 1000) / freq;
}

SFX::SFX(std::string filepath):
	playing(false),
	playTimer(0)
{
    this->sfx = NULL;
	this->setSFX(filepath);
    this->load();
}
SFX::~SFX()
{
	if (this->sfx) Mix_FreeChunk(this->sfx);
}
bool SFX::play()
{
    if (!this->sfx)
        return false;

    if (this->playing)
    {
	    if (!this->playTimer.isDone())
		    return false;

	    this->playing = false; // done!
    }

    this->playTimer.setDelay(getChunkTimeMilliseconds(this->sfx));
    this->playTimer.startCounting();
    this->playing = true;

    if (Mix_PlayChannel(-1, this->sfx, 0) == -1)
    {
	    Log::warning("Mix_PlayChannel: Couldn't play SFX on '" +
	                 this->filepath + "'");
        return false;
    }

//  how can I make it call a member function when done? apparently it
//	doesnt work this way
//	Mix_HookSFXFinished(this->finishedPlaying);

    return true;
}
bool SFX::load()
{
	if (this->sfx) Mix_FreeChunk(this->sfx);

    this->sfx = Mix_LoadWAV(this->filepath.c_str());
    if (!this->sfx)
    {
        Log::error("Mix_LoadWAV: Couldn't load SFX on '" +
                   this->filepath + "'");
        return false;
    }

    return true;
}
void SFX::setSFX(std::string filepath)
{
    this->filepath = filepath;
    this->load();
}

