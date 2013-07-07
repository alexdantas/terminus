#include <sstream> // for getline()
#include "DropDownConsole.hpp"
#include "InputManager.hpp"

DropDownConsoleHistory::DropDownConsoleHistory(int maxSize):
    maxSize(maxSize),
    currentIndex(0),
    firstIndex(0),
    lastIndex(-1)
{ }

/// Returns the *num*-th argument of the string *string*.
///
/// It recognizes arguments as strings separated by spaces.
std::string arg(int num, std::string text)
{
    std::istringstream stream(text);
    std::string currentArg = "";

    for (int i = 0; i <= num; i++)
        std::getline(stream, currentArg, ' ');

    return currentArg;
}
int argc(std::string text)
{
    std::istringstream stream(text);
    std::string currentArg = "";

    int i = 0;
    while (std::getline(stream, currentArg, ' '))
        i++;

    return i;
}


void DropDownConsoleHistory::add(std::string text)
{
    // If there's no space, push everyone to the beginning...
    if (this->size() == this->maxSize)
    {
        int i;
        for (i = 0; i < (this->maxSize - 1); i++)
            this->history[i] = this->history[i + 1];

        // ...and then force one on the back
        this->history[i] = text;
    }
    else
    {
        // Otherwise, gently insert
        this->history.push_back(text);
        this->lastIndex++;
    }
}
int DropDownConsoleHistory::size()
{
    return (this->lastIndex + 1);
}

std::string DropDownConsoleHistory::get(int index)
{
    if ((index < this->firstIndex) ||
        (index > this->lastIndex))
        return "";

    this->currentIndex = index;
    return (this->history[index]);
}
std::string DropDownConsoleHistory::getFirst()
{
    if (this->lastIndex < 0)
        return "";

    this->currentIndex = this->firstIndex;

    return (this->history[this->currentIndex]);
}
std::string DropDownConsoleHistory::getLast()
{
    if (this->lastIndex < 0)
        return "";

    this->currentIndex = this->lastIndex;

    return (this->history[this->currentIndex]);
}
std::string DropDownConsoleHistory::getCurrent()
{
    if (this->lastIndex < 0)
        return "";

    return (this->history[this->currentIndex]);
}
std::string DropDownConsoleHistory::getNext()
{
    if (this->lastIndex < 0)
        return "";

    if ((this->currentIndex + 1) > (this->lastIndex))
        this->currentIndex = this->lastIndex;
    else
        this->currentIndex++;

    return (this->history[this->currentIndex]);
}
std::string DropDownConsoleHistory::getPrevious()
{
    if ((this->currentIndex - 1) < (this->firstIndex))
        this->currentIndex = this->firstIndex;
    else
        this->currentIndex--;

    return (this->history[this->currentIndex]);
}
void DropDownConsoleHistory::setMaxSize(int size)
{
    this->maxSize = size;

    // TODO
    // TRUNCATE IF THE CURRENT SIZE IS SMALLER
}
void DropDownConsoleHistory::clear()
{
    this->history.clear();
    this->currentIndex = 0;
    this->lastIndex = -1;
}

////////////////////////////////////////////////////////////////////

Command::Command(std::string command, int value):
    command(command),
    value(value),
    argc(0)
{ }

void Command::addArgument(std::string text)
{
    this->argc++;
    this->argv.push_back(text);
}

////////////////////////////////////////////////////////////////////

DropDownConsole::DropDownConsole(Font* font, int x, int y, int w, int h):
    x(x),
    y(y),
    width(w),
    height(h),
    scrollSpeed(0),
    state(DropDownConsole::INVISIBLE),
    font(font),
    surface(NULL),
    horizontalChars(0),
    verticalChars(0),
    keybinding(SDLK_F12),
    topOffset(0),
    prompt(">"),
    inputBuffer(""),
    outputHistory(60),
    commandHistory(30),
    curHistorySize(0),
    maxHistorySize(30),
    cursor(0),
    currentCommand(NULL),
    userJustSentACommand(false)
{
    // TODO
    // test if w/h is bigger than screen or something
    // also check positions and stuff
    DropDownConsole::print("________                               ________                                ");
    DropDownConsole::print("\\______ \\   _______    ____   ______   \\______ \\     ____   __  _  __   ____   ");
    DropDownConsole::print(" |    |  \\  \\_  __ \\  /  _ \\  \\____ \\   |    |  \\   /  _ \\  \\ \\/ \\/ /  /    \\  ");
    DropDownConsole::print(" |    `   \\  |  | \\/ (  <_> ) |  |_> >  |    `   \\ (  <_> )  \\     /  |   |  \\ ");
    DropDownConsole::print("/_______  /  |__|     \\____/  |   __/  /_______  /  \\____/    \\/\\_/   |___|  / ");
    DropDownConsole::print("        \\/                    |__|             \\/                          \\/  ");
}
DropDownConsole::~DropDownConsole()
{

}
void DropDownConsole::addCommand(std::string command, int value)
{
    Command* tmp = new Command(command, value);

    this->commands.push_back(tmp);
}
bool DropDownConsole::hasCommand()
{
    return (this->userJustSentACommand);
}
int DropDownConsole::getCommand()
{
    if (!(this->userJustSentACommand))
        return -1;

    // Pop command
    this->userJustSentACommand = false;

    if (!(this->currentCommand))
        return -1;

    return (this->currentCommand->value);
}
int DropDownConsole::getCommandArgsAmmount()
{
    return (this->currentCommand->argc);

    // std::istringstream stream(this->currentCommand->command);
    // std::string currentArg = "";

    // int i = 0;

    // while (std::getline(stream, currentArg, ' '))
    //     i++;

    // return i;
}
std::string DropDownConsole::getCommandArg(int num)
{
    return (this->currentCommand->argv.at(num));
    //arg(num, this->currentCommand->command));
}
void DropDownConsole::setDropDownKeybinding(int key)
{
    this->keybinding = key;
}
void DropDownConsole::setBackgroundColor(Color color)
{
    this->background = color;
}
void DropDownConsole::setForegroundColor(Color color)
{
    this->foreground = color;
    this->font->setColor(color);
}
void DropDownConsole::setPromptForegroundColor(Color color)
{
    this->promptForeground = color;
    this->font->setColor(color);
}
void DropDownConsole::setScrollSpeed(float speed)
{
    this->scrollSpeed = speed;
}
void DropDownConsole::setPrompt(std::string prompt)
{
    this->prompt = prompt;
}
void DropDownConsole::refresh()
{
    this->surface = SDL::newSurface(this->width, this->height);
    SDL_FillRect(this->surface, NULL, SDL::convertColorFormat(this->background));

    this->horizontalChars = (this->width)  / (this->font->getCharWidth());
    this->verticalChars   = (this->height) / (this->font->getCharHeight());
}
void DropDownConsole::render(float cameraX, float cameraY)
{
    if (!(this->isVisible()))
        return;

    // Background
    SDL_FillRect(this->surface, NULL, SDL::convertColorFormat(this->background));
    SDL_SetAlpha(this->surface, SDL_SRCALPHA, this->background.a());

    this->renderLines();

    // Placing everything onscreen

    SDL_Rect crop;
    crop.x = 0;
    crop.y = this->height - this->topOffset;
    crop.w = this->width;
    crop.h = this->topOffset;

    SDL_Rect position;
    position.x = this->x - cameraX;
    position.y = this->y - cameraY;

    SDL::renderSurface(this->surface, &crop, &position);

    // History text

    // Cursor

}
void DropDownConsole::update(float dt)
{
    this->updateInput();
    this->updateAnimation(dt);
}

bool DropDownConsole::isVisible()
{
    if (this->state == DropDownConsole::INVISIBLE)
        return false;

    // Even when it's scrolling down, it's considered visible.
    return true;
}
void DropDownConsole::printCommand(std::string text)
{
    this->commandHistory.add(text);
}
void DropDownConsole::print(std::string text)
{
    this->outputHistory.add(text);
}
void DropDownConsole::clearHistory()
{
    this->outputHistory.clear();
}

// Private local functions

void DropDownConsole::updateAnimation(float dt)
{
    switch (this->state)
    {
    case DropDownConsole::INVISIBLE:
        break;

    case DropDownConsole::VISIBLE:
        break;

    case DropDownConsole::SCROLLING_UP:
        this->topOffset -= (this->scrollSpeed * dt);
        break;

    case DropDownConsole::SCROLLING_DOWN:
        this->topOffset += (this->scrollSpeed * dt);
        break;

    default:
        break;
    }

    // Actually changing animation states
    if (this->topOffset <= 0)
    {
        this->topOffset = 0;
        this->state = DropDownConsole::INVISIBLE;
    }
    if (this->topOffset >= this->height)
    {
        this->topOffset = this->height;
        this->state = DropDownConsole::VISIBLE;
    }
}
void DropDownConsole::updateInput()
{
    InputManager* input = InputManager::getInstance();

    // The only thing in the game that will receive input
    // when the console is active is the console itself.
    //
    // Everything else will be locked (see the end of this
    // method).
    if (this->isVisible())
        input->unlock();

    // Show/hide the console
    if ((input->isKeyDown(this->keybinding)))
    {
        switch (this->state)
        {
        case DropDownConsole::INVISIBLE:
            this->state = DropDownConsole::SCROLLING_DOWN;
            break;

        case DropDownConsole::VISIBLE:
            this->state = DropDownConsole::SCROLLING_UP;
            break;

        case DropDownConsole::SCROLLING_DOWN:
            this->state = DropDownConsole::SCROLLING_UP;
            break;

        case DropDownConsole::SCROLLING_UP:
            this->state = DropDownConsole::SCROLLING_DOWN;
            break;

        default:
            break;
        }
    }

    // If this is invisible, everything should be able
    // to receive input.
    //
    // But regarding to the console's internal input, the
    // only acceptable key will be the one to toggle it's
    // drop-down state.
    if (!(this->isVisible()))
    {
        input->unlock();
        return;
    }

    if (input->isKeyDown(SDLK_UP))
        this->inputBuffer = this->commandHistory.getPrevious();

    if (input->isKeyDown(SDLK_DOWN))
        this->inputBuffer = this->commandHistory.getNext();

    if (input->isKeyDown(SDLK_LEFT))
        this->cursorMoveLeft();

    if (input->isKeyDown(SDLK_RIGHT))
        this->cursorMoveRight();

    if (input->isKeyDown(SDLK_DELETE))
        this->cursorDelete();

    if (input->isKeyDown(SDLK_BACKSPACE))
        this->cursorBackspace();

    if (input->isKeyDown(SDLK_HOME))
        this->cursorHome();

    if (input->isKeyDown(SDLK_END))
        this->cursorEnd();

    if ((input->isKeyPressed(SDLK_LCTRL)) ||
        (input->isKeyPressed(SDLK_RCTRL)))
    {
        if (input->isKeyDown(SDLK_a))
        {
            this->cursorHome();
            return;
        }
        if (input->isKeyDown(SDLK_e))
        {
            this->cursorEnd();
            return;
        }
        if (input->isKeyDown(SDLK_l))
        {
            this->clearHistory();
            return;
        }
        if (input->isKeyDown(SDLK_c))
        {
            this->cursorClear();
            return;
        }
    }

    if (input->isPrintableKeyDown())
        this->addStringOnCursor(input->getCurPrintableKey());

    if (input->isKeyDown(SDLK_RETURN))
    {
        this->parseCommandOnInput();
        this->print(this->inputBuffer);
        this->printCommand(this->inputBuffer);
        this->cursorClear();
    }

    // If we got to this point, the console is visible.
    // So everything else should not have input.
    input->lock();
}
void DropDownConsole::renderLines()
{
    SDL_Surface* tmp;
    SDL_Rect position;

    // Prompt
    this->font->setColor(this->promptForeground);
    tmp        = this->font->render(this->prompt);
    position.x = 0;
    position.y = (this->height - this->font->getCharHeight());

    SDL_BlitSurface(tmp, NULL, this->surface, &position);

    // Input Buffer
    this->font->setColor(this->foreground);
    tmp        = this->font->render(this->inputBuffer);
    position.x = (this->font->getCharWidth() * this->prompt.length());

    SDL_BlitSurface(tmp, NULL, this->surface, &position);

    // Cursor
    tmp = this->font->render("_");
    position.x = (this->font->getCharWidth() * this->prompt.length() * (this->cursor + 1));

    SDL_BlitSurface(tmp, NULL, this->surface, &position);


    // History lines
    int current = this->outputHistory.size();

    if (current <= 0)
        return;

    // How many lines I can show besides the prompt
    int visibleHistoryLines = (this->height / this->font->getCharHeight()); // -1);// minus
                                                                                //the prompt

    // While (I can print onscreen) AND (there's available lines to print)...
    for (int i = 0; i < visibleHistoryLines && current > 0; i++)
    {
        tmp = this->font->render(this->outputHistory.get(current - 1));
        position.x = 0;
        position.y = this->height - (this->font->getCharHeight() * (i + 2));

        // ...do show them!
        SDL_BlitSurface(tmp, NULL, this->surface, &position);
        current--;
    }
}
void DropDownConsole::addStringOnCursor(std::string string)
{
    this->inputBuffer.insert(this->cursor, string);
    this->cursor++;
}
void DropDownConsole::cursorMoveLeft()
{
    if (this->cursor == 0) return;

    this->cursor--;
}
void DropDownConsole::cursorMoveRight()
{
    this->cursor++;

    if (this->cursor > this->inputBuffer.length())
        this->cursor = this->inputBuffer.length();

}
void DropDownConsole::cursorHome()
{
    this->cursor = 0;
}
void DropDownConsole::cursorEnd()
{
    this->cursor = this->inputBuffer.length();
}
void DropDownConsole::cursorDelete()
{
    if (this->inputBuffer.length() > 0)
        this->inputBuffer.erase(this->cursor, 1);
}
void DropDownConsole::cursorBackspace()
{
    if (this->inputBuffer.length() > 0)
    {
        this->inputBuffer.erase(this->cursor - 1, 1);
        this->cursor--;
    }
}
void DropDownConsole::cursorClear()
{
    if (this->inputBuffer.length() > 0)
    {
        this->inputBuffer.clear();
        this->cursor = 0;
    }
}
void DropDownConsole::parseCommandOnInput()
{
    if (this->commands.size() <= 0) return;

    if (this->currentCommand)
        delete this->currentCommand;

    this->userJustSentACommand = true;

    // See first word of the input buffer and compare with
    // all commands stored in memory
    int i;
    int size = this->commands.size();
    for (i = 0; i < size; i++)
    {
        std::string firstWord = arg(0, this->inputBuffer);

        if (firstWord == this->commands[i]->command)
        {
            this->currentCommand = new Command(this->commands[i]->command,
                                               this->commands[i]->value);

            int argAmmount = argc(this->inputBuffer);

            for (int i = 0; i < argAmmount; i++)
                this->currentCommand->addArgument(arg(i, this->inputBuffer));

            break;
        }
    }
    if (i == size) // no command found
        this->currentCommand = NULL;
}


