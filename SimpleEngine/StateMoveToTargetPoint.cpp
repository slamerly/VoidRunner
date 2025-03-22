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
				checkRotation(bot);

				if (angle > 0.01f)
				{
					if (dirZ > 0)
					{
						yawSpeed = std::min(yawSpeed + stepYawSpeed, maxYawSpeed);
					}
					else if (dirZ < 0)
					{
						yawSpeed = std::min(yawSpeed - stepYawSpeed, -maxYawSpeed);
					}
				}
				else
				{
					yawSpeed = 0;
					alligned = true;
				}
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

				yawSpeed = 0;
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
		mc->setAngularSpeed(yawSpeed);

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

		alligned = false;
		closeToTarget = false;
		isArrived = false;
	}
}

void StateMoveToTargetPoint::checkRotation(Actor* bot)
{
	//std::cout << "currentTarget : " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().x << ", " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().y << ", " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().z << std::endl;

	Vector3 direction = bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition() - bot->getPosition();
	direction.normalize();

	Vector3 forw = bot->getForward();
	forw.normalize();

	angle = Vector3::dot(forw, direction);
	angle = Maths::acos(angle);

	//std::cout << "angle: " << angle * 100 << std::endl;

	Vector3 cros = Vector3::cross(forw, direction);
	dirZ = cros.z;

	std::cout << "dirZ: " << dirZ << std::endl;
}

float StateMoveToTargetPoint::angleBetweenVectors(const Vector3& v1, const Vector3& v2)
{
	float dotProduct = Vector3::dot(v1, v2);
	float magnitude1 = std::sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	float magnitude2 = std::sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
	return std::acos(dotProduct / (magnitude1 * magnitude2));
}
