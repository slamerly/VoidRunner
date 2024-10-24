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
	float getRollSpeed() const { return rollSpeed; }
	float getStrafeSpeed() const { return strafeSpeed; }

	void setForwardSpeed(float forwardSpeedP);
	void setUpSpeed(float upSpeedP);
	void setAngularSpeed(float angularSpeedP);
	void setPitchSpeed(float pitchSpeedP);
	void setRollSpeed(float rollSpeedP);
	void setStrafeSpeed(float strafeSpeedP);

	void update(float dt) override;


private:
	float forwardSpeed = 0;
	float upSpeed = 0;
	float angularSpeed = 0;
	float pitchSpeed = 0;
	float rollSpeed = 0;
	float strafeSpeed = 0;
};

