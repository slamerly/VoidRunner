#pragma once
#include "Actor.h"
#include "SphereActor.h"
#include "PhysicsSystem.h"
#include "MeshComponent.h"
#include "StateMachine.h"
#include "StateMoveToTargetPoint.h"

class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();

	void updateActor(float dt) override;

	bool isLeader() { return learder; }
	void setIsLeader(bool isLeader);

private:
	SphereActor* sphere;
	SphereActor* sphereR;
	SphereActor* sphereL;
	MoveComponent* moveComponent;
	MeshComponent* mc;

	// voir aussi animation
	float fowardSpeed = 300.0f;

	// Rotation
	bool newDirection();
	bool Rclear = false;
	bool Lclear = false;

	const float segmentLength = 1000.0f;
	int sens = 0;
	bool sensChoiced = false;

	// ===== State Machine =====
	StateMachine* stateM;
	StateMoveToTargetPoint* stateMoveTarget;

	bool learder = false;

	// Detection system
	bool detection();
	Actor* actorDetected = nullptr;
	const float focusDistance = 1000;
	Vector3 RBorder, LBorder;
	float distMtBMax, distBtBMax;
	PhysicsSystem::CollisionInfo infoDetect;

	void animation(float dt);


	// Dodge
	bool isSaveDir = false;
	bool isdodging = false;
	Vector3 saveDir;
	void dodge(float distBA);

	// Animation
	bool topHeight = false;
	bool topBig = false;
	bool topFinal = false;

	// Debug
	//Vector3 startR, startL, endR, endL;
	//PhysicsSystem::CollisionInfo infoR, infoL;
	//bool segCastR = false;
	//bool segCastL = false;
};

