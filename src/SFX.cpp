#include "SFX.hpp"
#include "Log.hpp"

SFX::SFX(std::string filepath)
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

