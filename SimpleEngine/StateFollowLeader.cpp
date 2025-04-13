#include "StateFollowLeader.h"
#include <iostream>
#include "Game.h"

void StateFollowLeader::enter(Actor* bot)
{
	std::cout << "StateFollowLeader enter" << std::endl;

	for (auto comp : bot->getActorComponents())
	{
		MoveComponent* testMoveComp = dynamic_cast<MoveComponent*>(comp);
		if (testMoveComp)
		{
			mc = testMoveComp;
		}
	}

	if (bot->getGame().getPatrolLeader() == nullptr)
		std::cerr << "Game patrolLeader is null." << std::endl;
	else
		currentTarget = bot->getGame().getPatrolLeader();
}

void StateFollowLeader::update(Actor* bot, float deltaTime)
{
	defineTargetPosition(bot);
	distToTarget(bot);

	if (!isArrived)
	{
		checkRotation(bot);
		if (!allignedYaw || !allignedPitch)
		{
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
		}
	}
}

void StateFollowLeader::exit(Actor* bot)
{
	std::cout << "StateFollowLeader exit" << std::endl;
}

int StateFollowLeader::getPriority()
{
	return 1;
}

void StateFollowLeader::setPatrolPosition(int patrolPos)
{
	patrolPosition = patrolPos;
}

void StateFollowLeader::distToTarget(Actor* bot)
{
	if (!targetPosition.equal(Vector3::zero))
	{
		float dist = Vector3::distance(bot->getPosition(), targetPosition);

		if (dist <= maxDistToDecelerate)
		{
			// without stop at checkpoint
			isArrived = true;
		}
		else
			isArrived = false;
	}
}

void StateFollowLeader::defineTargetPosition(Actor* bot)
{
	if (bot->getGame().getPatrolLeader())
	{
		// pair number on the right of the leader
		if (Maths::fmod(patrolPosition, 2) == 0)
		{
			targetPosition = bot->getGame().getPatrolLeader()->getRight() * 5000
				+ bot->getGame().getPatrolLeader()->getForward() * -5000
				+ bot->getGame().getPatrolLeader()->getPosition();
		}
		else
		{
			targetPosition = bot->getGame().getPatrolLeader()->getRight() * -5000
				+ bot->getGame().getPatrolLeader()->getForward() * -5000
				+ bot->getGame().getPatrolLeader()->getPosition();
		}
	}
}

void StateFollowLeader::checkRotation(Actor* bot)
{
	//std::cout << std::endl << "===========" << std::endl;

	Vector3 direction = targetPosition - bot->getPosition();
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

	if (angleYaw != 0)
		allignedYaw = false;
	if (anglePitch != 0)
		allignedPitch = false;

	//std::cout << "angleYaw: " << angleYaw << std::endl;
	//std::cout << "anglePitch: " << anglePitch << std::endl;
}
