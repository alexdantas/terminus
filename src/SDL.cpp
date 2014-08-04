#include <sstream>
#include "SDL.hpp"
#include "Config.hpp"
#include "Log.hpp"
#include "Graphics.hpp"
#include "Window.hpp"

// Must intialize static members out of the class
uint32_t     SDL::framerate       = 0;
uint32_t     SDL::framerate_delay = 0;
uint32_t     SDL::frame_delta     = 0;
bool         SDL::startedAudio    = false;
Timer        SDL::framerate_timer;
FPSmanager   SDL::fpsManager;

std::vector<std::pair<std::string, SDL_Surface*> > SDL::loadedImages;

bool SDL::init(uint32_t frameRate)
{
    // SDL General Initialization
    int retval = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO |
                          SDL_INIT_TIMER | SDL_INIT_CDROM);
    if (retval)
    {
        Log::error("SDL_Init: Couldn't start SDL");
        Log::error(std::string(SDL_GetError()));
        throw "SDL Init Fail";
    }

    // TTF
    if (TTF_Init())
    {
        Log::error("TTF_Init: Couldn't start TTF");
        Log::error(std::string(TTF_GetError()));
        throw "TTF Init Fail";
    }

    // This makes input-handling a LOT easier.
    SDL_EnableUNICODE(SDL_ENABLE);

    if (Config::hasMusic)
    {
        bool succeeded = SDL::initAudio();
        if (!succeeded)
            startedAudio = false;
        else
            startedAudio = true;
    }
    else
        startedAudio = false;

    // I saw this here: http://www.kekkai.org/roger/sdl/mixer/
    // but don't quite know what it does.
    //Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);

    // Framerate manager
    framerate       = frameRate;
    framerate_delay = (1000/framerate); // 1 second

    framerate_timer.start();

    // SDL's framerate manager SUCKS
    // SDL_initFramerate(&(fpsManager));
    // SDL_setFramerate(&(fpsManager), frameRate);

    return true;
}
void SDL::exit()
{
    if (startedAudio)
    {
        //Mix_Quit(); segmentation fault! why?

        Mix_CloseAudio();
    }

    SDL_EnableUNICODE(SDL_DISABLE);

    TTF_Quit();

    // SDL_Quit already does this for me
    //SDL_FreeSurface(screen);

    SDL_Quit();
}
bool SDL::initAudio()
{
    Mix_Init(MIX_INIT_OGG); // | MIX_INIT_MP3

    // TODO: how do I find out the optimal audio rate of a music?
    int       audio_rate     = Config::audioRate;
    Uint16    audio_format   = AUDIO_S16; // 16-bit stereo
    int       audio_channels = 2;
    int       audio_buffers  = 4096;
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
    {
        Log::warning("Mix_OpenAudio: Couldn't start Audio");
        return false;
    }
    return true;
}
void SDL::framerateWait()
{
    frame_delta = framerate_timer.delta();

    if ((frame_delta) < (framerate_delay))
        SDL::delay_ms((framerate_delay) - frame_delta);

    framerate_timer.restart();

// SDL's framerate manager SUCKS
//    SDL_framerateDelay(&(fpsManager));
}
Uint32 SDL::getFramerateDelay()
{
    return framerate_delay;
}
void SDL::delay_ms(int delay)
{
    SDL_Delay(delay);
}
float SDL::getDeltaSeconds()
{
    // frame_delta is the number of miliseconds between last frame and
    // this one.
    //
    // We'll transform it in SECONDS.

    return ((float)(frame_delta))/1000.0;
}
uint32_t SDL::getDelta()
{
    return (frame_delta);
}
SDL_Surface* SDL::loadBMP(std::string filename)
{
    return (SDL_LoadBMP(filename.c_str()));
}
SDL_Surface* SDL::loadImage(std::string filename, bool optimize)
{
	// If it's on the cache, merely return it
	for (size_t i = 0; i < SDL::loadedImages.size(); i++)
		if (SDL::loadedImages[i].first == filename)
		{
			Log::verbose("Hit cache for '" + filename + "'");
			return SDL::loadedImages[i].second;
		}


    SDL_Surface* tmpImage = NULL;

    tmpImage = IMG_Load(filename.c_str());
    if (!tmpImage)
    {
        Log::error("IMG_Load: Couldn't open image '" + filename + "'");
        Log::error("IMG_Load:" + std::string(IMG_GetError()));
        return NULL;
    }

    if (!optimize)
        return tmpImage;

    // Optimizing image to screen, while trying to mantain it's
    // transparency (alpha)
    SDL_Surface* image = NULL;

    if (tmpImage->format->Amask)
        image = SDL_DisplayFormatAlpha(tmpImage);
    else
        image = SDL_DisplayFormat(tmpImage);

    SDL::freeImage(tmpImage);

    SDL::loadedImages.push_back(std::pair<std::string, SDL_Surface*>(filename, image));
    return image;
}
void SDL::freeImage(SDL_Surface* image)
{
    if (image) SDL_FreeSurface(image);
}
void SDL::renderSurface(SDL_Surface* source, SDL_Rect* crop, SDL_Rect* position)
{
    // This is a dirty hack to make the game screen always centralize
    // on the window size.
    //
    // You see, EVERYTHING that gets shown on the screen (including
    // the text) must pass through this method here.
    //
    // So whenever we resize the screen, Window::width and
    // Window::height will change.
    //
    // Whenever they do, we'll make sure to place the whole "Game
    // Window" (all objects) at the right position.
    //
    // This is like a "global camera". Hope I'm not confusing you.

    int x = (Window::width  - Window::originalWidth)/2;
    int y = (Window::height - Window::originalHeight)/2;

    SDL_Rect tmp;
    tmp.x = position->x + x;
    tmp.y = position->y + y;

    SDL_BlitSurface(source, crop, Window::screen, &tmp);
}
bool SDL::isPrintable(SDLKey key)
{
    if (key == ' ' ||
        key >= 'a' || key <= 'z' ||
        key >= 'A' || key <= 'Z' ||
        key >= '0' || key <= '9')
        return true;
    else
        return false;
}
bool SDL::musicPlaying()
{
    if (!(SDL::startedAudio))
        return false;

    if (Mix_PlayingMusic())
        return true;
    else
        return false;
}
bool SDL::musicPaused()
{
    if (!(SDL::startedAudio))
        return false;

    if (Mix_PausedMusic())
        return true;
    else
        return false;
}
void SDL::pauseMusic()
{
    if (!(SDL::startedAudio))
        return;

    if (SDL::musicPlaying())
        Mix_PauseMusic();
}
void SDL::resumeMusic()
{
    if (!(SDL::startedAudio))
        return;

    if (SDL::musicPaused())
        Mix_ResumeMusic();
}
void SDL::stopMusic()
{
    if (!(SDL::startedAudio))
        return;

    if (SDL::musicPlaying() || SDL::musicPaused())
        Mix_HaltMusic();
}
bool SDL::hasAudio()
{
    return startedAudio;
}
int SDL::randomNumberBetween(int min, int max)
{
    // Just in case the called didn't read
    // the method's documentation.
    if (min > max)
    {
        // swap
        int tmp = max;
        max = min;
        min = tmp;
    }

    return (rand() % (max - min + 1) + min);
}
bool SDL::randomBool()
{
    // Is a random number between 0 and 10 divisible by two?
    return (((SDL::randomNumberBetween(0, 10)) % 2) == 0);
}
void SDL::showCursor()
{
    SDL_ShowCursor(SDL_ENABLE);
}
void SDL::hideCursor()
{
    SDL_ShowCursor(SDL_DISABLE);
}
void SDL::toggleCursor()
{
    if (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE)
        SDL::hideCursor();
    else
        SDL::showCursor();
}
bool SDL::ejectCDTray()
{
    SDL_CD* cdrom;

    if (SDL_CDNumDrives() <= 0)
    {
        Log::warning("SDL_CDNumDrives: No CD-ROM drives available");
        return false;
    }

    // The system's default CD-ROM is always drive 0
    cdrom = SDL_CDOpen(0);
    if (!cdrom)
    {
        Log::warning("SDL_CDOpen: Couldn't open default CD-ROM: " +
                     std::string(SDL_GetError()));
        return false;
    }

    if (SDL_CDEject(cdrom) == -1)
    {
        SDL_CDClose(cdrom);
        return false;
    }
    else
    {
        SDL_CDClose(cdrom);
        return true;
    }
}
void SDL::printSystemInfo()
{

    // Getting Video Driver information
    char namebuf[255];
    SDL_VideoDriverName(namebuf, 254);
    Log::log("Video Driver: " + std::string(namebuf));

    // Getting Joystick information
    int num = SDL_NumJoysticks();

    Log::log("There are " + SDL::intToString(num) +
             " Joystick(s) currently available.");

    for (int i = 0; i < num; i++)
        Log::log("Joystick no " + SDL::intToString(i + 1) +
                 ": " + std::string(SDL_JoystickName(i)));

    // Getting CD-ROM information
    num = SDL_CDNumDrives();

    Log::log("There are " + SDL::intToString(num) +
                    " CD-ROM Drive(s) currently available.");

    for (int i = 0; i < num; i++)
        Log::log("CD-ROM Drive no " + SDL::intToString(i + 1) +
                 ": " + std::string(SDL_CDName(i)));
}
void SDL::grabInput()
{
    SDL_WM_GrabInput(SDL_GRAB_ON);
}
void SDL::releaseInput()
{
    SDL_WM_GrabInput(SDL_GRAB_OFF);
}
void SDL::toggleGrabInput()
{
    if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_OFF)
        SDL::grabInput();
    else
        SDL::releaseInput();
}
void SDL::setMousePosition(uint16_t x, uint16_t y)
{
    SDL_WarpMouse(x, y);
}
std::string SDL::intToString(int num)
{
    std::stringstream ss;
    ss.str("");
    ss << num;

    std::string numStr = ss.str();
    return numStr;
}
int SDL::stringToInt(std::string text)
{
    std::istringstream ss(text);
    int num;

    ss >> num;
    return (num);
}


SDL_Surface* SDL::clip(SDL_Surface* original, SDL_Rect* clip)
{
    // Creates a new surface based on *clip*'s dimensions
    // SDL_Surface* tmp = SDL_CreateRGBSurface(original->flags,
    //                                         clip->w,
    //                                         clip->h,
    //                                         original->format->BitsPerPixel,
    //                                         original->format->Rmask,
    //                                         original->format->Gmask,
    //                                         original->format->Bmask,
    //                                         original->format->Amask);

    SDL_Surface* tmp = SDL_CreateRGBSurface(original->flags,
                                            clip->w,
                                            clip->h,
                                            Window::screen->format->BitsPerPixel,
                                            0, 0, 0, 0);

    // Copies original *surface*'s contents to the newly created one
    SDL_BlitSurface(original, clip, tmp, NULL);
    return tmp;
}
SDL_Surface* SDL::rotoZoom(SDL_Surface* surface, float angle, float scaleX, float scaleY)
{
    // if (!surface || angle < 0)
    //     return NULL;

    return (rotozoomSurfaceXY(surface, angle, scaleX, scaleY, SMOOTHING_ON));
}
SDL_Surface* SDL::newSurface(int width, int height)
{
    // I don't even know why the last 4 arguments (masks) must all be
    // 0. If it isn't like this, will start bugging the hell out of my
    // program.
    //
    // I literally spent two fucking days browsing through SDL forums
    // and newbie tutorials about surfaces to realize this strange
    // behaviour.
    return (SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA,
                                 width, height,
                                 32, 0, 0, 0, 0));
}
void SDL::deleteSurface(SDL_Surface* surface)
{
    if (surface)
        SDL_FreeSurface(surface);
}
uint32_t SDL::convertColorFormat(Color color)
{
    return (SDL_MapRGBA(Window::screen->format,
                        color.r(),
                        color.g(),
                        color.b(),
                        color.a()));
}

