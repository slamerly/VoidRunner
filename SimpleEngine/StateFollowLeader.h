#pragma once
#include "State.h"
#include "MoveComponent.h"
#include "TargetPoint.h"

class StateFollowLeader :
	public State
{
public:
	void enter(Actor* bot) override;
	void update(Actor* bot, float deltaTime) override;
	void exit(Actor* bot) override;

	int getPriority() override;

	void setPatrolPosition(int patrolPos);

private:
	MoveComponent* mc;
	Actor* currentTarget = nullptr;

	void distToTarget(Actor* bot);
	float maxDistToDecelerate = 3500.f;
	bool isArrived = false;

	int patrolPosition = 0;
	Vector3 targetPosition = Vector3::zero;
	void defineTargetPosition(Actor* bot);

	void checkRotation(Actor* bot);

	//=======================================================
	//						Movements
	//=======================================================
	// Max
	const float sensitiveRota = Maths::twoPi;


	// ===== Forward =====
	// Forward - Init
	float forwardSpeed = 0.0f;

	// Forward - Max
	float maxFowardSpeed = 4000.0f;
	float maxNegatFowardSpeed = maxFowardSpeed / 2;

	// Forward - Steps
	const float stepForwardSpeed = 40.0f;
	const float stepForwardBreak = stepForwardSpeed * 1.25f;
	const float stepNegateForwardSpeed = stepForwardSpeed / 2;

	// Forward - Exp
	const float initWExpStepForward = stepForwardSpeed * 1.25f;
	const float initSExpStepForward = stepForwardBreak * 2.5f;
	const float initSExpNegStepForward = 0;
	float currentExpStepForward = initWExpStepForward;


	// ===== Yaw =====
	// Yaw - Init
	float yawSpeed = 0.0f;
	float angleYaw = 0;
	bool allignedYaw = false;

	// Yaw - Max
	const float maxYawSpeed = sensitiveRota / 10;

	// Yaw - Steps
	const float stepYawSpeed = .01f;

	// ===== Pitch =====
	// Pitch - Init
	float pitchSpeed = 0.0f;
	float anglePitch = 0;
	bool allignedPitch = false;

	// Pitch - Max
	const float maxPitchSpeed = sensitiveRota / 10;

	// Pitch - Steps
	const float stepPitchSpeed = .01f;
};

