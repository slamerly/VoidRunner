#include "MoveComponent.h"
#include "Maths.h"
#include "Actor.h"
#include "Window.h"
#include <iostream>

MoveComponent::MoveComponent(Actor* ownerP, int updateOrderP)
	: Component(ownerP, updateOrderP), forwardSpeed(0.0f), upSpeed(0.0f), angularSpeed(0.0f), pitchSpeed(0.0f), rollSpeed(0.0f), strafeSpeed(0.0f)
{

}

void MoveComponent::setForwardSpeed(float forwardSpeedP)
{
	forwardSpeed = forwardSpeedP;
}

void MoveComponent::setUpSpeed(float upSpeedP)
{
	upSpeed = upSpeedP;
}

void MoveComponent::setAngularSpeed(float angularSpeedP)
{
	angularSpeed = angularSpeedP;
}

void MoveComponent::setPitchSpeed(float pitchSpeedP)
{
	pitchSpeed = pitchSpeedP;
}

void MoveComponent::setRollSpeed(float rollSpeedP)
{
	rollSpeed = rollSpeedP;
}

void MoveComponent::setStrafeSpeed(float strafeSpeedP)
{
	strafeSpeed = strafeSpeedP;
}

void MoveComponent::update(float dt)
{
	if (!Maths::nearZero(angularSpeed) || !Maths::nearZero(pitchSpeed) || !Maths::nearZero(rollSpeed))
	{
		Quaternion newRotation = owner.getRotation();

		if (!Maths::nearZero(rollSpeed)) // X (roulis)
		{
			float rollAngle = rollSpeed * dt;
			Quaternion rollIncrement(owner.getForward(), rollAngle);
			newRotation = Quaternion::concatenate(newRotation, rollIncrement);
		}

		if (!Maths::nearZero(pitchSpeed)) // Y (tangage)
		{
			float pitchAngle = pitchSpeed * dt;
			Quaternion pitchIncrement(owner.getRight(), pitchAngle);
			newRotation = Quaternion::concatenate(newRotation, pitchIncrement);
		}

		if (!Maths::nearZero(angularSpeed)) // Z (lacet)
		{
			float yawAngle = angularSpeed * dt;
			Quaternion yawIncrement(Vector3::unitZ, yawAngle);
			newRotation = Quaternion::concatenate(yawIncrement, newRotation);
		}

		owner.setRotation(newRotation);
	}

	if (!Maths::nearZero(forwardSpeed) || !Maths::nearZero(strafeSpeed) || !Maths::nearZero(upSpeed))
	{
		Vector3 newPosition = owner.getPosition();
		newPosition += owner.getForward() * forwardSpeed * dt;
		newPosition += owner.getRight() * strafeSpeed * dt;
		newPosition += owner.getUp() * upSpeed * dt;
		owner.setPosition(newPosition);
	}
}
