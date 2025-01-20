#pragma once
#include "Actor.h"
#include "SphereActor.h"

class Character : public Actor
{
public:
	Character();
	~Character();

	void actorInput(const struct InputState& inputState) override;
	void updateActor(float dt) override;

	void shoot();
	void reload();

	void fixCollisions();

private:
	class MoveComponent* moveComponent;
	class MeshComponent* mc;
	class Camera* cameraComponent;
	//class Actor* FPSModelRifle;
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
	const float maxFowardSpeed = 4000.0f;
	const float maxNegatFowardSpeed = maxFowardSpeed / 2;
	const float maxUpSpeed = maxNegatFowardSpeed;
	const float maxStrafeSpeed = 3000.0f;
	const float maxMouseSpeed = 500;
	const float sensitiveRota = Maths::twoPi;

	// Steps
	const float stepForwardSpeed = 50.0f;
	const float stepNegateForwardSpeed = stepForwardSpeed / 2;

	float forwardSpeed = 0.0f;
	
	bool readyToMoveBack = false;
	bool onForwardDelay = false;
	float forwardDelay = 5 * 60.0f;
	
	// Ammo
	int magazineMax = 15;
	int currentMagazine = 0;

	// Reload
	bool isReloading = false;
	bool decending = true;
	float finalAngle = 0.35f;
	float currentAngle = 0.0f;
	float timeReload = 2.5f;
	float currentTimeReload = 0.0f;

	bool isShooting = false;
	float cooldownShoot = 0.15f;
	float currentCooldownShoot = 0.f;
	float ballDamage = 5;
};

const Vector3 MODEL_OFFSET = Vector3(20.0f, 30.0f, 10.0f);