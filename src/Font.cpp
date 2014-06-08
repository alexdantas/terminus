#include "Font.hpp"
#include "Log.hpp"

Font::Font(std::string filepath, int size, RenderType renderType, Color color):
    font(NULL),
    surface(NULL)
{
    this->setRenderType(renderType);
	this->setFont(filepath, size);
	this->setColor(color);
}
Font::~Font()
{
//	Why does this gives me a Segmentation Fault?
//	if (this->font)	   TTF_CloseFont(this->font);
	if (this->surface) SDL_FreeSurface(this->surface);
    Log::verbose("Font deleted");
}
bool Font::load()
{
	if (this->font) TTF_CloseFont(this->font);

	this->font = TTF_OpenFont(this->filepath.c_str(), this->size);
	if (!this->font)
    {
        Log::error("Couldn't load font '" + this->filepath + "': " +
                   std::string(SDL_GetError()));
		return false;
    }

    // Assuming monospaced fonts!
    int w, h;
    if (TTF_SizeText(this->font, "a", &w, &h))
    {
        Log::warning("Couldn't get font size:" +
                     std::string(TTF_GetError()));
        w = 0;
        h = 0;
    }

    this->charWidth  = w;
    this->charHeight = h;

    Log::debug("Font::load '" + this->filepath + "': (" +
               SDL::intToString(w) + ", " + SDL::intToString(h) + ")");

    // TODO i can also load a BMP font and that way giving it's width
    //      and height would be a piece of cake

	return true;
}
void Font::setFont(std::string filepath, int size)
{
	this->filepath = filepath;
	this->size = size;
    this->load();
}
void Font::setColor(Color color)
{
	this->color = color;
}
void Font::setRenderType(RenderType renderType)
{
    this->renderType = renderType;
}
bool Font::print(int x, int y, std::string text, FontStyle style)
{
    if (!this->font) return false;

    // Getting FONT style
	int cur_style = TTF_STYLE_NORMAL;

    switch (style)
    {
    case BOLD:      cur_style = TTF_STYLE_BOLD;      break;
    case ITALIC:    cur_style = TTF_STYLE_ITALIC;    break;
    case UNDERLINE: cur_style = TTF_STYLE_UNDERLINE; break;
    case NORMAL: default: break;
    }

	// we can use | to set several styles
	// maybe i should create a nicer interface to it
	TTF_SetFontStyle(this->font, cur_style);

    // Getting SURFACE style
	if (this->surface) SDL_FreeSurface(this->surface);

	SDL_Color tmpColor;
	tmpColor.r = this->color.r();
	tmpColor.g = this->color.g();
	tmpColor.b = this->color.b();

    const char* ctext = text.c_str();
    switch (this->renderType)
    {
    case BLENDED:
        this->surface = TTF_RenderText_Blended(this->font, ctext, tmpColor);
        break;
    case SOLID: default:
        this->surface = TTF_RenderText_Solid(this->font, ctext, tmpColor);
        break;
    }
	if (!this->surface)
    {
        Log::warning("TTF_RenderText_* Couldn't render text '" + text + "'");
		return false;
    }

	// Getting WHERE to render
	SDL_Rect tmpRect;
	tmpRect.x = x;
	tmpRect.y = y;

    // Actually rendering
	SDL::renderSurface(this->surface, NULL, &tmpRect);
	return true;
}
SDL_Surface* Font::render(std::string text)
{
	SDL_Color tmpColor;
	tmpColor.r = this->color.r();
	tmpColor.g = this->color.g();
	tmpColor.b = this->color.b();

    return (TTF_RenderText_Solid(this->font, text.c_str(), tmpColor));
}
// I made these based on this tutorial:
// http://www.sdltutorials.com/sdl-ttf
// but it doesn't seem to be working, DAMN
//
// void Font::setOutline(int size)
// {
//	TTF_SetFontOutline(this->font, size);
// }
// int	Font::getOutline()
// {
//	   TTF_GetFontOutline(this->font);
// }
// void Font::resetOutline()
// {
//	   this->setOutline(0);
// }

int Font::getCharWidth()
{
    return (this->charWidth);
}
int Font::getCharHeight()
{
    return (this->charHeight);
}
void Font::bold(bool option)
{
    // Bitwise adding/removing attributes

    int style = TTF_GetFontStyle(this->font);
    if (option)
        style |=  TTF_STYLE_BOLD;
    else
        style &= ~TTF_STYLE_BOLD;

    TTF_SetFontStyle(this->font, style);
}
void Font::italic(bool option)
{
    int style = TTF_GetFontStyle(this->font);
    if (option)
        style |=  TTF_STYLE_ITALIC;
    else
        style &= ~TTF_STYLE_ITALIC;

    TTF_SetFontStyle(this->font, style);
}
void Font::underline(bool option)
{
    int style = TTF_GetFontStyle(this->font);
    if (option)
        style |=  TTF_STYLE_UNDERLINE;
    else
        style &= ~TTF_STYLE_UNDERLINE;;

    TTF_SetFontStyle(this->font, style);
}

