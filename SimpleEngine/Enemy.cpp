#include "Enemy.h"
#include "Assets.h"
#include "BoxCollisionComponent.h"
#include "LineSegment.h"
#include "Game.h"
#include "MoveComponent.h"
#include "BallActor.h"

Enemy::Enemy()
{
	setRotation(Quaternion(Vector3::unitZ, Maths::pi));

	mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Corvette"));

	moveComponent = new MoveComponent(this);
	//BoxCollisionComponent* bc = new BoxCollisionComponent(this);
	//bc->setObjectBox(Assets::getMesh("Destroyer_01").getBox());

	// State Machine
	stateM = new StateMachine();
	stateMoveTarget = new StateMoveToTargetPoint();
	stateM->addState(stateMoveTarget);
	stateFollow = new StateFollowLeader();
	stateM->addState(stateFollow);

	if(learder)
		stateM->changeState(this, stateMoveTarget);
	else
		stateM->changeState(this, stateFollow);

	// ===== DEBUG =====
	
	//sphere = new SphereActor();
	//sphere->setScale(5.0f);
	
	sphereR = new SphereActor();
	sphereR->setScale(25.0f);
	sphereR->getMeshComponent()->setTextureIndex(4);
	sphereL = new SphereActor();
	sphereL->setScale(25.0f);
	sphereL->getMeshComponent()->setTextureIndex(3);
	

	//moveComponent = new MoveComponent(this);
	//moveComponent->setForwardSpeed(fowardSpeed);
	//moveComponent->setForwardSpeed(0);

	srand(time(nullptr));

	getGame().addMovableActor(this);
}

Enemy::~Enemy()
{
	//sphere->setState(Actor::ActorState::Dead);
	//sphereR->setState(Actor::ActorState::Dead);
	//sphereL->setState(Actor::ActorState::Dead);
	getGame().removeMovableActor(this);
}

void Enemy::updateActor(float dt)
{
	stateM->update(this, dt);

	if (learder)
	{
		sphereL->setPosition(getRight() * -5000 + getForward() * -5000 + getPosition());
		sphereR->setPosition(getRight() * 5000 + getForward() * -5000 + getPosition());
	}

	/*
	Vector3 start = getPosition() + getForward() * 100.0f;
	Vector3 dir = getForward();
	Vector3 end = start + dir * segmentLength;

	RBorder = start + getRight() * 500.0f + dir * (segmentLength * 0.5f);
	LBorder = start + getRight() * -500.0f + dir * (segmentLength * 0.5f);

	//sphere->setPosition(end);

	LineSegment l(start, end);

	PhysicsSystem::CollisionInfo info;
	if (getGame().getPhysicsSystem().segmentCast(l, info) && info.actor != this)
	{
		//Character* target = dynamic_cast<Character*>(info.actor);
		PlaneActor* mur = dynamic_cast<PlaneActor*>(info.actor);
		if (mur && info.distance < 400)
		{
			//std::cout << "mur" << std::endl;
			moveComponent->setForwardSpeed(0);
			if (!sensChoiced)
			{
				sens = rand() % 2;
				sensChoiced = true;
			}

			if(sens == 0)
				moveComponent->setAngularSpeed(-1.f);
			else
				moveComponent->setAngularSpeed(1.f);
		}
		//sphere->setPosition(info.point);
	}
	*/

	// --- Debug ---
	/*
	startR = getPosition() + getRight() * 100.0f;
	endR = startR + getRight() * 390.0f + getForward() * segmentLength;
	startL = getPosition() + getRight() * -1.f * 100.0f;
	endL = startL + getRight() * -390.0f + getForward() * segmentLength;

	LineSegment lR(startR, endR);
	LineSegment lL(startL, endL);

	segCastR = getGame().getPhysicsSystem().segmentCast(lR, infoR);
	segCastL = getGame().getPhysicsSystem().segmentCast(lL, infoL);
	
	if (segCastR && infoR.actor != this)
	{
		sphereR->setPosition(infoR.point);
		sphereR->getMeshComponent()->setTextureIndex(3);
	}
	else
	{
		sphereR->setPosition(endR);
		sphereR->getMeshComponent()->setTextureIndex(4);
	}

	if (segCastL && infoL.actor != this)
	{
		sphereL->setPosition(infoL.point);
		sphereL->getMeshComponent()->setTextureIndex(3);
	}
	else
	{
		sphereL->setPosition(endL);
		sphereL->getMeshComponent()->setTextureIndex(4);
	}
	*/
	// --- End Debug ---

	//sphere->setPosition(getForward() * 1000 + getPosition());

	//if (sensChoiced && !isdodging)
	//{
	//	moveComponent->setAngularSpeed(0);
	//	moveComponent->setForwardSpeed(fowardSpeed);
	//	sensChoiced = false;
	//	Rclear = false;
	//	Lclear = false;
	//}
	//else
	//{
	//	//sphereL->setPosition(Vector3(0, 0, -1500));
	//	//sphereR->setPosition(Vector3(0, 0, -1500));
	//}
	//std::cout << newDirection() << std:: endl;
	animation(dt);
}

void Enemy::setIsLeader(bool isLeader)
{
	learder = isLeader;
	getGame().setPatrolLeader(this);
	stateM->changeState(this, stateMoveTarget);
}

void Enemy::setCrewNumber(int newCrewNumber)
{
	crewNumber = newCrewNumber;

	if (stateFollow)
	{
		stateFollow->setPatrolPosition(newCrewNumber);
	}
}

bool Enemy::detection()
{
	/*
	distMtBMax = dist3D(getPosition(), RBorder);
	distBtBMax = dist3D(RBorder, LBorder);
	float distance = INFINITY;
	for( auto movableActor : getGame().getMovableActors())
	{
		float currentDistance = dist3D(getPosition(), movableActor->getPosition());

		//if (currentDistance <= focusDistance && currentDistance < distance && movableActor != this)
		if (currentDistance <= focusDistance && 
			currentDistance < distance && 
			movableActor != this)
		{
			distance = currentDistance;
			actorDetected = movableActor;
		}
	}

	if (actorDetected != nullptr && dist3D(getPosition(), actorDetected->getPosition()) > focusDistance)
	{
		actorDetected = nullptr;
	}

	if (actorDetected != nullptr)
	{
		Vector3 startDetect = getPosition() + getForward() * 100.0f;
		Vector3 endDetect = actorDetected->getPosition();

		LineSegment lDetect(startDetect, endDetect);

		//sphere->setPosition(endDetect);

		if (getGame().getPhysicsSystem().segmentCast(lDetect, infoDetect) && infoDetect.actor != this)
		{
			float distRBtP = dist3D(RBorder, actorDetected->getPosition());
			float distLBtP = dist3D(LBorder, actorDetected->getPosition());
			float distMtP = dist3D(getPosition(), actorDetected->getPosition());

			if (distRBtP < distBtBMax && distLBtP < distBtBMax && distMtP < distMtBMax)
			{
				
				// Player
				Character* player = dynamic_cast<Character*>(infoDetect.actor);
				if (player)
				{
					
				}
				
			}
			else
			{

				moveComponent->setAngularSpeed(0);
				moveComponent->setForwardSpeed(fowardSpeed);
			}

			// Enemy
			Enemy* enemy = dynamic_cast<Enemy*>(infoDetect.actor);
			if (enemy)
			{
				if ((infoDetect.distance < 500) || infoDetect.distance < 200)
					isdodging = true;
			}
		}
	}
	// Dodging
	if (isdodging)
	{
		if (!isSaveDir)
		{
			saveDir = getForward();
			isSaveDir = true;
		}
		dodge(infoDetect.distance);
	}
	*/
	return false;
}

void Enemy::animation(float dt)
{
}

void Enemy::dodge(float distBA)
{
	/*if ((round(getForward().x * 100) >= round(-saveDir.x * 100 - 5)) && (round(getForward().x * 100) <= round(-saveDir.x * 100 + 5))
		&& (round(getForward().y * 100) >= round(-saveDir.y * 100 - 5)) && (round(getForward().y * 100) <= round(-saveDir.y * 100 + 5)))
	{
		moveComponent->setForwardSpeed(fowardSpeed);
		moveComponent->setAngularSpeed(0.f);
		isdodging = false;
		isSaveDir = false;
	}
	else
	{
		moveComponent->setForwardSpeed(0);
		moveComponent->setAngularSpeed(1.f);
	}*/
}
