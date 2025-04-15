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
		if(!allignedYaw || !allignedPitch)
		{ 
			checkRotation(bot);

			// ===== Yaw =====
			if (Maths::abs(angleYaw) > 0.01f)
			{
				yawSpeed = Maths::clamp(yawSpeed + stepYawSpeed * (angleYaw > 0 ? 1 : -1), -maxYawSpeed, maxYawSpeed);
			}
			else
			{
				yawSpeed = 0;
				allignedYaw = true;
			}
				
			// ===== Pitch =====
			if (Maths::abs(anglePitch) > 0.01f) {
				pitchSpeed = Maths::clamp(pitchSpeed + stepPitchSpeed * (anglePitch < 0 ? 1 : -1), -maxPitchSpeed, maxPitchSpeed);
			}
			else {
				pitchSpeed = 0;
				allignedPitch = true;
			}
		}

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

		mc->setForwardSpeed(forwardSpeed);
		mc->setAngularSpeed(yawSpeed);
		mc->setPitchSpeed(pitchSpeed);

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

void StateMoveToTargetPoint::setCurrentExpStepForward(float expStepForward)
{
	currentExpStepForward = expStepForward;
}

void StateMoveToTargetPoint::setCurrentForwardSpeed(float forwSpeed)
{
	forwardSpeed = forwSpeed;
}

void StateMoveToTargetPoint::distToTarget(Actor* bot)
{
	if (currentTarget)
	{
		float dist = Vector3::distance(bot->getPosition(), currentTarget->getPosition());

		if (dist <= maxDistToDecelerate)
		{
			// without stop at checkpoint
			isArrived = true;
		}
		else
			isArrived = false;
	}
}

void StateMoveToTargetPoint::nextTargetPoint(Actor* bot)
{
	nextTargetIsSelected = true;

	if (!bot->getGame().getTargetPoints().empty())
	{
		if (isIncreamentTarget)
		{
			currentIndexTargetPoint++;

			if (currentIndexTargetPoint <= bot->getGame().getTargetPoints().size() - 1)
				currentTarget = bot->getGame().getTargetPoints()[currentIndexTargetPoint];
			else
			{
				isIncreamentTarget = false;
				nextTargetPoint(bot);
				//currentIndexTargetPoint = 0;
				//currentTarget = bot->getGame().getTargetPoints()[currentIndexTargetPoint];
			}
		}
		else
		{
			currentIndexTargetPoint--;

			if (currentIndexTargetPoint >= 0)
				currentTarget = bot->getGame().getTargetPoints()[currentIndexTargetPoint];
			else
			{
				isIncreamentTarget = true;
				nextTargetPoint(bot);
			}
		}

		allignedYaw = false;
		allignedPitch = false;
		isArrived = false;
		nextTargetIsSelected = false;
	}
}

void StateMoveToTargetPoint::checkRotation(Actor* bot)
{
	//std::cout << std::endl << "===========" << std::endl;

	Vector3 direction = bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition() - bot->getPosition();
	direction.normalize();
	
	//std::cout << "direction: " << direction.toString() << std::endl;

	Vector3 forw = bot->getForward();
	forw.normalize();

	Quaternion invRotation = Quaternion::inverse(bot->getRotation());
	Vector3 dirLocal = direction.rotate(invRotation);
	dirLocal.normalize();

	//std::cout << "dirLocal: " << dirLocal.toString() << std::endl;

	angleYaw = atan2(dirLocal.y, dirLocal.x);
	anglePitch = atan2(dirLocal.z, dirLocal.x);

	//std::cout << "angleYaw: " << angleYaw << std::endl;
	//std::cout << "anglePitch: " << anglePitch << std::endl;
}
