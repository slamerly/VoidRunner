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

	if (bot->getGame().getTargetPoints().empty())
		std::cerr << "Game TargetPoints is empty." << std::endl;
	else
		currentTarget = bot->getGame().getTargetPoints()[0];
}

void StateMoveToTargetPoint::update(Actor* bot, float deltaTime)
{
	if (!isArrived)
	{
		if (!closeToTarget)
		{
			if(!alligned)
			{ 
				float te = Vector3::dot(bot->getPosition(), currentTarget->getPosition());
				//std::cout << te << std::endl;
				
			}
			else
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
		}
		else
		{
			if (forwardSpeed > 0)
			{
				forwardSpeed -= stepForwardSpeed;
			}
			if (forwardSpeed <= 0)
			{
				forwardSpeed = 0;
				currentExpStepForward = 0;
				isArrived = true;
			}
		}
		mc->setForwardSpeed(forwardSpeed);

		distToTarget(bot);
	}
	else
	{
		if (!nextTargetIsSelected)
		{
			nextTargetPoint(bot);
		}
	}
}

void StateMoveToTargetPoint::exit(Actor* bot)
{
	std::cout << "StateMoveToTargetPoint exit" << std::endl;
}

int StateMoveToTargetPoint::getPriority()
{
	return 1;
}

void StateMoveToTargetPoint::distToTarget(Actor* bot)
{
	if (currentTarget)
	{
		float dist = Vector3::distance(bot->getPosition(), currentTarget->getPosition());
		if (dist <= maxDistToDecelerate)
		{
			//std::cout << "Close" << std::endl;
			closeToTarget = true;
		}
	}
}

void StateMoveToTargetPoint::nextTargetPoint(Actor* bot)
{
	nextTargetIsSelected = true;
	currentIndexTargetPoint++;
	if (!bot->getGame().getTargetPoints().empty())
	{
		if(currentIndexTargetPoint <= bot->getGame().getTargetPoints().size() - 1)
			currentTarget = bot->getGame().getTargetPoints()[currentIndexTargetPoint];
		else
		{
			// TODO: faire le trajet retour.
			
		}

		checkRotation(bot);

		alligned = false;
		closeToTarget = false;
		isArrived = false;
	}
}

void StateMoveToTargetPoint::checkRotation(Actor* bot)
{
	std::cout << "currentTarget : " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().x << ", " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().y << ", " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().z << std::endl;

	Vector3 direction = bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition() - bot->getPosition();
	direction.normalize();

	std::cout << "direction : " << direction.toString() << std::endl;

	std::cout << "forward : " << bot->getForward().toString() << std::endl;

	Vector3 forw = bot->getForward();
	forw.normalize();

	float angle = Vector3::dot(forw, direction);
	angle = Maths::acos(angle);
	//float angle = Maths::atan2(Vector3::cross(forw, direction).length(), Vector3::dot(forw, direction));

	std::cout << "angle: " << angle << std::endl;

	bot->setAngle(Vector3::unitZ, angle);
}
