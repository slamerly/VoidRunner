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
	SphereActor* sphereX;
	SphereActor* sphereY;

	//=======================================================
	//						Movements
	//=======================================================
	// Debug
	bool debugMovement = false;

	// Max
	const float maxUpSpeed = maxNegatFowardSpeed;
	const float maxStrafeSpeed = 3000.0f;
	const float maxMouseSpeed = 500;
	const float sensitiveRota = Maths::twoPi;

	// UI
	vector<SpriteComponent*> UISpeed;

	// ===== Forward =====
	// Forward - Init
	float forwardSpeed = 0.0f;
	bool readyToMoveBack = false;

	// Forward - Max
	const float maxFowardSpeed = 4000.0f;
	const float maxNegatFowardSpeed = maxFowardSpeed / 2;

	// Forward - Steps
	const float stepForwardSpeed = 40.0f;
	const float stepForwardBreak = stepForwardSpeed * 1.25f;
	const float stepNegateForwardSpeed = stepForwardSpeed / 2;

	// Forward - Exp
	const float initWExpStepForward = stepForwardSpeed * 1.25f;
	const float initSExpStepForward = stepForwardBreak * 2.5f;
	const float initSExpNegStepForward = 0;
	float currentExpStepForward = initWExpStepForward;


	// ===== Strafe =====
	// Strafe - Init
	float strafeSpeed = 0.0f;
	bool canReturn = false;

	// Strafe - Steps
	const float stepStrafeSpeed = 30.0f;
};

const Vector3 MODEL_OFFSET = Vector3(20.0f, 30.0f, 10.0f);