#pragma once
#include "UIScreen.h"

class PauseMenu : public UIScreen
{
public:
	PauseMenu();
	~PauseMenu();

	void processInput(const class InputState& inputState) override;
};

