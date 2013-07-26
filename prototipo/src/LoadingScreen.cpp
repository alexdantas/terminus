#include "LoadingScreen.hpp"
#include "Window.hpp"

LoadingScreen::LoadingScreen(std::string title, std::string subtitle, int amount):
    progressBar(390, 570, 400, 20, 100, 0),
    bg(NULL),
    loadingBar(NULL),
    amount(amount),
    percent(0)
{
    this->loadingBar   = new Sprite("img/loading-bar.png");
    this->titleFont    = new Font("ttf/UbuntuMono.ttf", 42);
    this->subtitleFont = new Font("ttf/UbuntuMono.ttf", 20);

    this->title.setFont(this->titleFont);
    this->subtitle.setFont(this->subtitleFont);

    this->title.setText(title);
    this->title.setPosition(390, 504);

    this->subtitle.setText(subtitle);
    this->subtitle.setPosition(390, 545);

    this->progressBar.setBackgroundColor(Color(10, 95, 10));
    this->progressBar.setForegroundColor(Color(20, 255, 20));

    Window::clear();
    if (this->bg) this->bg->render();
    this->title.render();
    this->subtitle.render();

    // rendering progress bar for the first time
    this->increase(0);

    if(this->amount != 0)
        this->percent = 100/amount;
}
LoadingScreen::~LoadingScreen()
{
    if (this->titleFont)    delete this->titleFont;
    if (this->subtitleFont) delete this->subtitleFont;
}
void LoadingScreen::increase(unsigned int ammount)
{
    if(percent != 0)
        this->progressBar.increase(percent);
    else
        this->progressBar.increase(ammount);

    this->progressBar.render();
    this->loadingBar->render(387,565);
    Window::refresh();
}
void LoadingScreen::set(unsigned int ammount)
{
    this->progressBar.set(ammount);
    this->progressBar.render();
    Window::refresh();
}
void LoadingScreen::setTitle(std::string text)
{
    this->title.setText(text);

    Window::clear();
    if (this->bg) this->bg->render();
    this->title.render();
    this->subtitle.render();
    this->increase(0);
}
void LoadingScreen::setSubtitle(std::string text)
{
    this->subtitle.setText(text);

    Window::clear();
    if (this->bg) this->bg->render();
    this->title.render();
    this->subtitle.render();
    this->increase(0);
}
void LoadingScreen::setBg(std::string filepath)
{
    if (this->bg) delete this->bg;

    this->bg = new Sprite(filepath);
}

