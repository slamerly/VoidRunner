#include "StateMoveToTargetPoint.h"
#include <iostream>
#include "Game.h"

void StateMoveToTargetPoint::enter(Actor* bot)
{
	std::cout << "StateMoveToTargetPoint enter" << std::endl;

	for (auto comp : bot->getActorComponents())
	{
		MoveComponent* testMoveComp = dynamic_cast<MoveComponent*>(comp);
		if (testMoveComp)
		{
			mc = testMoveComp;
		}
	}

	distToTarget(bot);

	if (bot->getGame().getTargetPoints().empty())
		std::cerr << "Game TargetPoints is empty." << std::endl;
}

void StateMoveToTargetPoint::update(Actor* bot, float deltaTime)
{
	if(!closeToTarget)
	{
		currentExpStepForward++;

		// ===== Move Forward =====
		if (forwardSpeed < maxFowardSpeed)
		{
			if (exp(currentExpStepForward / stepForwardSpeed) > stepForwardSpeed)
			{
				forwardSpeed += stepForwardSpeed;
			}
			else
			{
				//std::cout << currentExpStepForward << std::endl;
				forwardSpeed += exp(currentExpStepForward / stepForwardSpeed);
			}

			if (forwardSpeed > maxFowardSpeed)
			{
				forwardSpeed = maxFowardSpeed;
			}
		}
	}

	mc->setForwardSpeed(forwardSpeed);
}

void StateMoveToTargetPoint::exit(Actor* bot)
{
	std::cout << "StateMoveToTargetPoint exit" << std::endl;
}

int StateMoveToTargetPoint::getPriority()
{
	return 4;
}

void StateMoveToTargetPoint::distToTarget(Actor* bot)
{
	bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition();
	/*float dist = Vector3::distance(bot->getPosition(), currentTargetPoint->getPosition());
	
	std::cout << "Distance : " << dist << std::endl;

	if (dist <= maxDistToDecelerate)
	{
		closeToTarget = true;
	}*/
}
