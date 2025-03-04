#pragma once
#include "Actor.h"
class State
{
public:
	virtual ~State() = default;

	virtual void enter(Actor* bot) = 0;
	virtual void update(Actor* bot, float deltaTime) = 0;
	virtual void exit(Actor* bot) = 0;

	virtual int getPriority() { return 0; }
};

