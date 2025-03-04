#include "StateMachine.h"
#include <iostream>

StateMachine::StateMachine()
{
	currentState = nullptr;
}

void StateMachine::update(Actor* bot, float deltaTime)
{
	if (currentState)
	{
		currentState->update(bot, deltaTime);
	}
}

void StateMachine::changeState(Actor* bot, State* newState)
{
	if (currentState)
	{
		currentState->exit(bot);
	}

	if (newState)
	{
		currentState = newState;
		currentState->enter(bot);
	}
	else
	{
		std::cout << "newState not valide." << std::endl;
	}
}
