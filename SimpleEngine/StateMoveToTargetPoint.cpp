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
			if(!allignedYaw || !allignedPitch)
			{ 
				checkRotation(bot);

				if (Maths::abs(angleYaw) > 0.01f)
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
					allignedYaw = true;
				}

				if (Maths::abs(anglePitch) > 0.001f) {
					if (dirY >= 0) {
						pitchSpeed = std::max(pitchSpeed - stepPitchSpeed, -maxPitchSpeed);
					}
					else {
						pitchSpeed = std::min(pitchSpeed + stepPitchSpeed, maxPitchSpeed);
					}
				}
				else {
					pitchSpeed = 0;
					allignedPitch = true;
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

		allignedYaw = false;
		allignedPitch = false;
		closeToTarget = false;
		isArrived = false;
	}
}

void StateMoveToTargetPoint::checkRotation(Actor* bot)
{
	//std::cout << "currentTarget : " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().x << ", " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().y << ", " << bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition().z << std::endl;
	std::cout << std::endl << "===========" << std::endl;

	Vector3 direction = bot->getGame().getTargetPoints()[currentIndexTargetPoint]->getPosition() - bot->getPosition();
	direction.normalize();
	
	std::cout << "direction: " << direction.toString() << std::endl;

	Vector3 forw = bot->getForward();
	forw.normalize();

	std::cout << "forw: " << forw.toString() << std::endl;


	// Yaw
	//angle = Vector3::dot(forw, direction);
	//angle = Maths::acos(angle);

	//angleYaw = Vector2::dot(Vector2(forw.x, forw.y), Vector2(direction.x, direction.y));
	//angleYaw = Maths::acos(angleYaw);

	//anglePitch = Vector3::dot(Vector3(forw.x, 0, forw.z), Vector3(direction.x, 0, direction.z));
	//anglePitch = Vector2::dot(Vector2(forw.x, forw.z), Vector2(direction.x, direction.z));
	//anglePitch = Vector2::dot(Vector2(forw.z, Maths::sqrt(forw.x * forw.x + forw.y * forw.y)),
	//	Vector2(direction.z, Maths::sqrt(direction.x * direction.x + direction.y * direction.y))); // Pitch
	//anglePitch = Vector2::dot(Vector2(forw.z, 0.0f), Vector2(direction.z, Maths::sqrt(direction.x * direction.x + direction.y * direction.y))); // Pitch

	anglePitch = Maths::atan2(direction.z, Maths::sqrt(direction.x * direction.x + direction.y * direction.y)) -
		Maths::atan2(forw.z, Maths::sqrt(forw.x * forw.x + forw.y * forw.y)); // Correction pour Pitch

	//std::cout << "angle: " << angle * 100 << std::endl;

	Vector3 cros = Vector3::cross(forw, direction);
	dirZ = cros.z;
	dirY = cros.y;
	//dirY = (Maths::abs(anglePitch) > 0.01f) ? (direction.z - forw.z) : cros.y;

	if (Maths::abs(anglePitch) < 0.001f && Maths::abs(direction.z - forw.z) > 0.01f) {
		anglePitch = direction.z - forw.z > 0 ? 0.01f : -0.01f; // Ajout d'une correction minimale
	}

	//std::cout << "dirZ: " << dirZ << std::endl;
	std::cout << "dirY: " << dirY << std::endl;
	//std::cout << "angleYaw: " << angleYaw << std::endl;
	std::cout << "anglePitch: " << anglePitch << std::endl;
}
