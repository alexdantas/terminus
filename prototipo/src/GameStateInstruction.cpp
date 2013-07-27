#include "GameStateInstruction.hpp"
#include "InputManager.hpp"
#include "Window.hpp"

GameStateInstruction::GameStateInstruction():
    image(NULL)
{ }
void GameStateInstruction::load(int stack)
{
    this->image      = new Sprite("img/instruction.png");

}
int GameStateInstruction::unload()
{
    if (this->image)  delete this->image;

    return 0;
}
GameState::StateCode GameStateInstruction::update(float dt)
{
    InputManager* input = InputManager::getInstance();
    input->update();

    if (input->isKeyDown(SDLK_RETURN))
        return GameState::MAIN_MENU;

    if ((input->isKeyDown(SDLK_q)) ||
        (input->isKeyDown(SDLK_ESCAPE)))
        return GameState::QUIT;

    if (input->quitRequested())
        return GameState::QUIT;

    // if we're not doing anything it keeps TRAVANDO
    return GameState::CONTINUE;
}
void GameStateInstruction::render()
{
    this->image->render();
}


