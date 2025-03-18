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

private:
    MoveComponent* mc;
	int currentIndexTargetPoint = 0;
	Actor* currentTarget = nullptr;

	void distToTarget(Actor* bot);
	bool closeToTarget = false;
	float maxDistToDecelerate = 3500.f;
	bool isArrived = false;

	bool alligned = true;

	void nextTargetPoint(Actor* bot);
	bool nextTargetIsSelected = false;

	void checkRotation(Actor* bot);

    //=======================================================
    //						Movements
    //=======================================================
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
};

