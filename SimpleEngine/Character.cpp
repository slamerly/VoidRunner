#include "Character.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "Game.h"
#include "CubeActor.h"
#include "BallActor.h"
#include "BoxCollisionComponent.h"
#include "Collisions.h"

Character::Character() : Actor(), moveComponent(nullptr), cameraComponent(nullptr), boxComponent(nullptr)
{
	moveComponent = new MoveComponent(this);
	cameraComponent = new Camera(this);

	//FPSModelRifle = new Actor();
	//FPSModelRifle->setScale(0.75f);
	//mc = new MeshComponent(FPSModelRifle);
	//mc->setMesh(Assets::getMesh("Mesh_Ak74"));

	// ===== DEBUG =====
	/*
	sphereX = new SphereActor();
	sphereX->setScale(1.0f);
	sphereX->getMeshComponent()->setTextureIndex(3);
	
	sphereY = new SphereActor();
	sphereY->setScale(1.0f);
	sphereY->getMeshComponent()->setTextureIndex(4);
	*/

	boxComponent = new BoxCollisionComponent(this);
	AABB collision(Vector3(-25.0f, -25.0f, -87.5f), Vector3(25.0f, 25.0f, 87.5f));
	boxComponent->setObjectBox(collision);
	boxComponent->setShouldRotate(false);

	currentMagazine = magazineMax;

	getGame().addMovableActor(this);
}

Character::~Character()
{
	getGame().removeMovableActor(this);
}

void Character::actorInput(const struct InputState& inputState)
{
	//std::cout << getPosition().x << ", " << getPosition().y << std::endl;
	//float forwardSpeed = 0.0f;
	float upSpeed = 0.0f;
	float angularSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	// wasd movement
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
	{
		if (debugMovement)
			forwardSpeed += maxFowardSpeed;
		else
			if (forwardSpeed < maxFowardSpeed)
			{
				forwardSpeed += stepForwardSpeed;
				std::cout << "Current Forward speed: " << forwardSpeed << std::endl;
			}
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
	{
		if (debugMovement)
		forwardSpeed -= maxNegatFowardSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_SPACE))
	{
		if (debugMovement)
		upSpeed += maxUpSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_LCTRL))
	{
		if (debugMovement)
		upSpeed -= maxUpSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
	{
		if (cameraComponent->getFPScam())
		{
			if (debugMovement)
				strafeSpeed -= maxStrafeSpeed;
		}
		else
			angularSpeed -= sensitiveRota;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		if (cameraComponent->getFPScam())
		{
			if (debugMovement)
				strafeSpeed += maxStrafeSpeed;
		}
		else
			angularSpeed += sensitiveRota;
	}

	// Camera shake movement
	/*
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W) ||
		inputState.keyboard.getKeyValue(SDL_SCANCODE_S) ||
		inputState.keyboard.getKeyValue(SDL_SCANCODE_A) ||
		inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		cameraComponent->setShakeSpeed(0.18f);
	}
	else
		cameraComponent->setShakeSpeed(0.09f);
	*/
	
	moveComponent->setForwardSpeed(forwardSpeed);
	moveComponent->setUpSpeed(upSpeed);
	moveComponent->setAngularSpeed(angularSpeed);
	moveComponent->setStrafeSpeed(strafeSpeed);

	// FPS Camera
	if (cameraComponent->getFPScam())
	{
		Vector2 mousePosition = inputState.mouse.getPosition();
		float x = mousePosition.x;
		float y = mousePosition.y;

		
		const float maxAngularSpeed = Maths::pi * 8;

		float angularSpeed = 0.0f;
		if (x != 0)
		{
			if (debugMovement)
			{
				angularSpeed = x / maxMouseSpeed;
				angularSpeed *= maxAngularSpeed;
			}
		}
		moveComponent->setAngularSpeed(angularSpeed);
		
		// PITCH
		const float maxPitchSpeed = Maths::pi * 8;
		float pitchSpeed = 0.0f;
		if (y != 0)
		{
			if (debugMovement)
			{
				pitchSpeed = y / maxMouseSpeed;
				pitchSpeed *= maxPitchSpeed;
			}
		}
		moveComponent->setPitchSpeed(pitchSpeed);

		// Roll
		float rollSpeed = 0.0f;
		if (inputState.keyboard.getKeyValue(SDL_SCANCODE_Q))
		{
			if (debugMovement)
				rollSpeed += sensitiveRota / 2;
		}
		if (inputState.keyboard.getKeyValue(SDL_SCANCODE_E))
		{
			if (debugMovement)
				rollSpeed -= sensitiveRota / 2;
		}
		moveComponent->setRollSpeed(rollSpeed);
		
	}

	if (inputState.mouse.getButtonState(1) == ButtonState::Pressed || inputState.mouse.getButtonState(1) == ButtonState::Held)
	{
		//shoot();
	}
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_R) == ButtonState::Pressed)
	{
		//reload();
	}
}

void Character::updateActor(float dt)
{
	Actor::updateActor(dt);

	//sphereX->setPosition(getPosition() + getForward() * 300);
	//sphereY->setPosition(getPosition() + getForward() * 300 + getRight() * 100);
	
	// Update position and rotation of model relatively to position
	Vector3 modelPosition = getPosition();

	// === Shoot Animation ===
	if (isShooting)
	{
		modelPosition += getForward() * (MODEL_OFFSET.x + currentCooldownShoot * 50);
		if (currentCooldownShoot > 0)
			currentCooldownShoot -= dt;
		else
			isShooting = false;
	}
	else
	{
		modelPosition += getForward() * MODEL_OFFSET.x;
		
	}
	modelPosition += getRight() * MODEL_OFFSET.y;
	modelPosition.z += MODEL_OFFSET.z;
	//FPSModelRifle->setPosition(modelPosition);
	Quaternion q = getRotation();

	// === RELOADING ===
	if (isReloading && !isShooting)
	{
		if (currentAngle < (cameraComponent->getPitch() + finalAngle) && decending)
		{
			currentAngle += dt;
		}
		else if (currentTimeReload < timeReload)
		{
			currentTimeReload += dt;
			decending = false;
		}
		else if (currentAngle > cameraComponent->getPitch())
		{
			currentAngle -= dt;
		}
		else
		{
			currentAngle = 0.0f;
			currentTimeReload = 0.0f;
			currentMagazine = magazineMax;
			isReloading = false;
		}
	}

	//q = Quaternion::concatenate(q, Quaternion(getRight(), cameraComponent->getPitch() + currentAngle));

	//FPSModelRifle->setRotation(q);
	
	fixCollisions();
}

void Character::shoot()
{
	if (currentMagazine > 0 && !isReloading && !isShooting)
	{		
		// Get start point (in center of screen on near plane)
		Vector3 screenPoint(445.0f, -270.0f, 0.0f);
		//Vector3 start = getGame().getRenderer().unproject(screenPoint);
		//Vector3 start = FPSModelRifle->getPosition() + FPSModelRifle->getForward() * 75;
		Vector3 start = getGame().getRenderer().unproject(screenPoint) + Vector3(0.f, 10.f, .5f);
		// Get end point (in center of screen, between near and far)
		screenPoint.z = 0.9f;
		//screenPoint = Vector3(75.5f, -45.0f, 0.9f);
		Vector3 end = getGame().getRenderer().unproject(screenPoint);
		// Get direction vector
		Vector3 dir = end;
		dir.normalize();

		// Spawn a ball
		BallActor* ball = new BallActor(ballDamage);
		ball->getMeshComponent()->setTextureIndex(1);
		ball->setPlayer(this);
		ball->setPosition(start + dir * 20.0f);
		// Rotate the ball to face new direction
		ball->rotateToNewForward(dir);

		isShooting = true;
		currentCooldownShoot = cooldownShoot;
		currentMagazine--;
	}
}

void Character::reload()
{
	decending = true;
	isReloading = true;
}

void Character::fixCollisions()
{
	// Need to recompute world transform to update world box
	computeWorldTransform();

	const AABB& playerBox = boxComponent->getWorldBox();
	Vector3 pos = getPosition();

	auto& planes = getGame().getPlanes();
	for (auto pa : planes)
	{
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->getBox()->getWorldBox();
		if (Collisions::intersect(playerBox, planeBox))
		{
			// Calculate all our differences
			float dx1 = planeBox.max.x - playerBox.min.x;
			float dx2 = planeBox.min.x - playerBox.max.x;
			float dy1 = planeBox.max.y - playerBox.min.y;
			float dy2 = planeBox.min.y - playerBox.max.y;
			float dz1 = planeBox.max.z - playerBox.min.z;
			float dz2 = planeBox.min.z - playerBox.max.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Maths::abs(dx1) < Maths::abs(dx2) ? dx1 : dx2;
			// Ditto for dy
			float dy = Maths::abs(dy1) < Maths::abs(dy2) ? dy1 : dy2;
			// Ditto for dz
			float dz = Maths::abs(dz1) < Maths::abs(dz2) ? dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Maths::abs(dx) <= Maths::abs(dy) && Maths::abs(dx) <= Maths::abs(dz))
			{
				pos.x += dx;
			}
			else if (Maths::abs(dy) <= Maths::abs(dx) && Maths::abs(dy) <= Maths::abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			setPosition(pos);
			boxComponent->onUpdateWorldTransform();
		}
	}
}
