#include "MoveComponent.h"
#include "Maths.h"
#include "Actor.h"
#include "Window.h"
#include <iostream>

MoveComponent::MoveComponent(Actor* ownerP, int updateOrderP)
	: Component(ownerP, updateOrderP), forwardSpeed(0.0f), upSpeed(0.0f), angularSpeed(0.0f), pitchSpeed(0.0f), yawSpeed(0.0f), strafeSpeed(0.0f)
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

void MoveComponent::setYawSpeed(float yawSpeedP)
{
	yawSpeed = yawSpeedP;
}

void MoveComponent::setStrafeSpeed(float strafeSpeedP)
{
	strafeSpeed = strafeSpeedP;
}

void MoveComponent::update(float dt)
{
	if (!Maths::nearZero(angularSpeed))
	{
		Quaternion newRotation = owner.getRotation();
		float angle = angularSpeed * dt;
		Quaternion increment(Vector3::unitZ, angle);
		//Quaternion increment(owner.getUp(), angle);
		newRotation = Quaternion::concatenate(newRotation, increment);
		owner.setRotation(newRotation);
	}
	if (!Maths::nearZero(pitchSpeed))
	{
		Quaternion newRotation = owner.getRotation();
		float angle = pitchSpeed * dt;
		//Quaternion increment(owner.getRight(), angle);
		Quaternion increment(Vector3::unitY, angle);
		Quaternion incr(Vector3::unitX, 0);
		newRotation = Quaternion::concatenate(increment, newRotation);
		//newRotation = Quaternion::concatenate(newRotation, incr);
		std::cout << "Rotation: " << owner.getRotation().x << ", " << owner.getRotation().y << ", " << owner.getRotation().z << ", " << owner.getRotation().w << std::endl;
		owner.setRotation(newRotation);
	}
	if (!Maths::nearZero(yawSpeed))
	{
		Quaternion newRotation = owner.getRotation();
		float angle = yawSpeed * dt;
		//Quaternion increment(owner.getRight(), angle);
		Quaternion increment(Vector3::unitX, angle);
		newRotation = Quaternion::concatenate(newRotation, increment);
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
