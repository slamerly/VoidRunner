#include "StateMoveToTargetPoint.h"
#include <iostream>

void StateMoveToTargetPoint::enter(Actor* bot)
{
	std::cout << "StateMoveToTargetPoint enter" << std::endl;
}

void StateMoveToTargetPoint::update(Actor* bot, float deltaTime)
{
}

void StateMoveToTargetPoint::exit(Actor* bot)
{
	std::cout << "StateMoveToTargetPoint exit" << std::endl;
}

int StateMoveToTargetPoint::getPriority()
{
	return 4;
}
