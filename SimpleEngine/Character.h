#pragma once
#include "Actor.h"
#include "SphereActor.h"
#include "SpriteComponent.h"

class Character : public Actor
{
public:
	Character();
	~Character();

	void actorInput(const struct InputState& inputState) override;
	void updateActor(float dt) override;

	void fixCollisions();

private:
	class MoveComponent* moveComponent;
	class MeshComponent* mc;
	class Camera* cameraComponent;
	class BoxCollisionComponent* boxComponent;

	// Test
	//SphereActor* sphereX;
	//SphereActor* sphereY;

	//=======================================================
	//						Movements
	//=======================================================
	// Debug
	bool debugMovement = true;
	float debugPower = 5.f;

	// Max
	const float maxMouseSpeed = 500;
	const float sensitiveRota = Maths::twoPi;

	// UI
	vector<SpriteComponent*> UIForwardSpeed;
	vector<SpriteComponent*> UIRightSpeed;
	vector<SpriteComponent*> UIUpSpeed;
	int uiSpeedIndexForward = 0;
	int uiSpeedIndexRight = 0;
	int uiSpeedIndexUp = 0;

	// ===== Forward =====
	// Forward - Init
	float forwardSpeed = 0.0f;
	bool readyToMoveBack = false;

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


	// ===== Strafe Right =====
	// Strafe Right - Init
	float strafeRightSpeed = 0.0f;
	bool canReturn = false;

	// Strafe Right - Max
	float maxStrafeRightSpeed = 3000.0f;

	// Strafe Right - Steps
	const float stepStrafeRightSpeed = 30.0f;


	// ===== Strafe Up =====
	// Strafe Up - Init
	float strafeUpSpeed = 0.0f;
	bool canStabilized = false;

	// Strafe Up - Max
	float maxUpSpeed = maxNegatFowardSpeed;

	// Strafe Right - Steps
	const float stepStrafeUpSpeed = 50.0f;


	// ===== Yaw =====
	// Yaw - Init
	float angularSpeed = 0.0f;
	float currentPowerAngularSpeed = 0.0f;

	// Yaw - Max
	const float maxAngularSpeed = Maths::pi * 8;

	// Yaw - Max
	const float stepAngularSpeed = Maths::pi;


	// ===== Pitch =====
	// Pitch - Init
	float pitchSpeed = 0.0f;
	float currentPowerPitchSpeed = 0.0f;

	// Pitch - Max
	const float maxPitchSpeed = Maths::pi * 8;

	// Pitch - Max
	const float stepPitchSpeed = Maths::pi;


	// ===== Roll =====
	// Roll - Init
	float rollSpeed = 0.0f;
	bool rollReleased = false;

	// Roll - Max
	const float maxRollSpeed = sensitiveRota / 2;

	// Roll - Steps
	const float stepRollSpeed = .075f;

	// Roll - Exp
	float currentExpStepRoll = 0.0f;
	const float initExpStepRoll = stepRollSpeed * 1.25f;
};

const Vector3 MODEL_OFFSET = Vector3(20.0f, 30.0f, 10.0f);