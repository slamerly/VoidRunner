#pragma once
#include "Actor.h"
#include "SphereActor.h"
#include "PhysicsSystem.h"
#include "MeshComponent.h"
#include "StateMachine.h"
#include "StateMoveToTargetPoint.h"
#include "StateFollowLeader.h"

class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();

	void updateActor(float dt) override;

	bool getIsLeader() { return learder; }
	void setIsLeader(bool isLeader);
	void setCrewNumber(int newCrewNumber);
	int getCrewNumber() { return crewNumber; }

	void updateCrewNumber();

private:
	MoveComponent* moveComponent;
	MeshComponent* mc;

	// voir aussi animation
	float fowardSpeed = 300.0f;

	// Rotation
	bool Rclear = false;
	bool Lclear = false;

	const float segmentLength = 1000.0f;
	int sens = 0;
	bool sensChoiced = false;

	// ===== State Machine =====
	StateMachine* stateM;
	StateMoveToTargetPoint* stateMoveTarget;
	StateFollowLeader* stateFollow;

	bool learder = false;
	int crewNumber = 0;

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
	void debug(bool enable);
	bool isDebug = false;
	SphereActor* sphere;
	SphereActor* sphereR;
	SphereActor* sphereL;
	//Vector3 startR, startL, endR, endL;
	//PhysicsSystem::CollisionInfo infoR, infoL;
	//bool segCastR = false;
	//bool segCastL = false;
};

