#pragma once
#include "State.h"

class StateMachine
{
public:
	StateMachine();

	void update(Actor* bot, float deltaTime);
	void changeState(Actor* bot, State* newState);

private:
	State* currentState;

};

