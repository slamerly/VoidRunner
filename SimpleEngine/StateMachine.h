#pragma once
#include "State.h"
#include <map>

class StateMachine
{
public:
	StateMachine();


	void addState(State* state);
	void update(Actor* bot, float deltaTime);
	void changeState(Actor* bot, State* newState);

private:
	State* currentState;
	std::map<State*, int> statePriorityMap;
	int currentStatePriority = 0;
};

