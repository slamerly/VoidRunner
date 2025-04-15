#pragma once
#include "State.h"
#include "MoveComponent.h"
#include "TargetPoint.h"

class StateMoveToTargetPoint :
    public State
{
public:
    void enter(Actor* bot) override;
    void update(Actor* bot, float deltaTime) override;
    void exit(Actor* bot) override;

    int getPriority() override;

	void setCurrentExpStepForward(float expStepForward);
	void setCurrentForwardSpeed(float forwSpeed);

private:
    MoveComponent* mc;
	int currentIndexTargetPoint = 0;
	Actor* currentTarget = nullptr;

	void distToTarget(Actor* bot);
	float maxDistToDecelerate = 3500.f;
	bool isArrived = false;

	void nextTargetPoint(Actor* bot);
	bool nextTargetIsSelected = false;
	bool isIncreamentTarget = true;

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
	float currentExpStepForward = initWExpStepForward;


	// ===== Yaw =====
	// Yaw - Init
	float yawSpeed = 0.0f;
	float angleYaw = 0;
	bool allignedYaw = false;

	// Yaw - Max
	const float maxYawSpeed = sensitiveRota / 15;

	// Yaw - Steps
	const float stepYawSpeed = .01f;

	// ===== Pitch =====
	// Pitch - Init
	float pitchSpeed = 0.0f;
	float anglePitch = 0;
	bool allignedPitch = false;

	// Pitch - Max
	const float maxPitchSpeed = sensitiveRota / 15;

	// Pitch - Steps
	const float stepPitchSpeed = .01f;
};

