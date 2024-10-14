#include "PauseMenu.h"
#include "Game.h"
#include "DialogBox.h"

PauseMenu::PauseMenu() : UIScreen()
{
	Game::instance().setState(GameState::Pause);

	Game::instance().getInputSystem().setMouseCursor(true);

	Game::instance().getInputSystem().setMouseRelativeMode(false);

	setTitle("Pause");
	addButton("ResumeButton", [this]() { close(); });
	//addButton("QuitButton", [this]() { Game::instance().setState(GameState::Quit); });
	
	addButton("QuitButton", [this]() { 
		new DialogBox("QuitText", [this]() {
			Game::instance().setState(GameState::Quit); });
		});
}

PauseMenu::~PauseMenu()
{
	Game::instance().setState(GameState::Gameplay);
}

void PauseMenu::processInput(const InputState& inputState)
{
	UIScreen::processInput(inputState);
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Released)
	{
		close();
	}
}