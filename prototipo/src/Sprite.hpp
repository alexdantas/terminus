#ifndef SPRITE_H_DEFINED
#define SPRITE_H_DEFINED

#include <iostream>
#include "SDL.hpp"
#include "Color.hpp"

/// Shows a single image onscreen.
///
///	This class opens an image and can be used to print it completely
///	or just a part of it.
///
///	For example, on the class *Animation*, we'll deal with opening a
///	large image and selectively printing parts of it.
///
/// ## How to use:
///
///	    Sprite sprite("path/to/image.png");
///         /// on a loop...
///	        sprite.render(50, 50);
///
///	Keep in mind you should refresh the screen to see any changes.
///
class Sprite
{
public:
	Sprite(std::string filename);
	virtual ~Sprite();

	/// Assigns a new filename to the current sprite.
	//	@note Remember to call load() after this.
	void setFilename(std::string newFilename);

	/// Loads sprite's image on memory (specified by #filename).
	//	This overrides previous loadings. It means that you can
	//	reload an image (or set a new image through setFilename())
	//	without worrying about the previous one.
	bool load();

	/// Crops image to the rectangle specified by #x, #y, #w and #h
	void crop(int x, int y, int w, int h);

	/// Places the sprite onscreen.
	//	Screen needs to be refreshed, though.
	virtual void render(int x=0, int y=0);

    virtual void update(float dt);

	/// Sets a RGB color on this sprite as transparent.
	//	This means when printing this image, this color will be
	//	passed-through and look invisible.
	void setTransparentRGBColor(Color color=Color(255, 0, 255));

    /// Returns the width of the sprite image on pixels.
	int getWidth();

    /// Returns the height of the sprite image on pixels.
	int getHeight();

    void rotoZoom(float angle, float scaleX=1.0, float scaleY=1.0, bool force=false);

    void restore();

    /// Returns current image's clip on the SDL's internal
    /// representation of a rectangle.
    SDL_Rect* getClip();

    void clip(int x, int y, int w, int h);

    /// Sets image's transparency (alpha channel) to *level*.
    ///
    /// 255 is opaque, 0 is completely transparent.
    void setTransparency(uint8_t level);

protected:
	/// Current image's filename.
	///
	///	Can be changed with setFilename().
	///	Remember to load() it after.
	std::string	 filename;

	/// SDL's internal thingy that holds the image data.
	SDL_Surface* surface;

    /// In case we apply some sort of rotation or zoom on the
    /// original surface, this is where it'll be saved.
    SDL_Surface* surfaceRotoZoom;

	/// Which part of the image is printed onscreen.
	//
	//	Normally it's the whole image, but you can clip()
	//	it if you like.
	SDL_Rect* clipRect;

    /// If we've rotoZoomed the image, this is the clip we're
    /// using to print it onscreen.
    SDL_Rect rotoZoomClipRect;

    float angle;
    float scaleX;
    float scaleY;
};

#endif /* SPRITE_H_DEFINED */
