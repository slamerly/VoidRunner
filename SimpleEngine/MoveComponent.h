#pragma once
#include "Component.h"
class MoveComponent : public Component
{
public:
	MoveComponent(Actor* ownerP, int updateOrder = 10); // By default, update before other components
	MoveComponent() = delete;
	MoveComponent(const MoveComponent&) = delete;
	MoveComponent& operator=(const MoveComponent&) = delete;

	float getForwardSpeed() const { return forwardSpeed; }
	float getUpSpeed() const { return upSpeed; }
	float getAngularSpeed() const { return angularSpeed; }
	float getPitchSpeed() const { return pitchSpeed; }
	float getYawSpeed() const { return yawSpeed; }
	float getStrafeSpeed() const { return strafeSpeed; }

	void setForwardSpeed(float forwardSpeedP);
	void setUpSpeed(float upSpeedP);
	void setAngularSpeed(float angularSpeedP);
	void setPitchSpeed(float pitchSpeedP);
	void setYawSpeed(float yawSpeedP);
	void setStrafeSpeed(float strafeSpeedP);

	void update(float dt) override;


private:
	float forwardSpeed;
	float upSpeed;
	float angularSpeed;
	float pitchSpeed;
	float yawSpeed;
	float strafeSpeed;

	float pitch = 0;
};

