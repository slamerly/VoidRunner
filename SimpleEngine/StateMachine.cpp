#include "StateMachine.h"
#include <iostream>

StateMachine::StateMachine()
{
	currentState = nullptr;
}

void StateMachine::addState(State* state)
{
	statePriorityMap.insert(std::pair<State*, int>(state, state->getPriority()));
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
	int newStatePriority = 0;

	if (!statePriorityMap.empty())
		newStatePriority = statePriorityMap.find(newState)->second;
	else
		std::cout << "statePriorityMap is empty" << std::endl;

	if (newStatePriority >= currentStatePriority)
	{
		if (currentState)
		{
			currentState->exit(bot);
		}

		if (newState)
		{
			currentState = newState;
			currentStatePriority = newStatePriority;
			currentState->enter(bot);
		}
		else
		{
			std::cout << "newState not valide." << std::endl;
		}
	}
}
