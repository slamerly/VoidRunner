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

	getGame().addMovableActor(this);

	float step = 36.0f;

	// UI
	for (int i = 0; i < 15; i++)
	{
		Actor* ui = new Actor();
		ui->setPosition(Vector3(-900.0f, -162.0f + i * step, 0.0f));
		SpriteComponent* sc = new SpriteComponent(ui, Assets::getTexture("Speed"));
		sc->setVisible(false);

		UIForwardSpeed.push_back(sc);
	}

	for (int i = 0; i < 20; i++)
	{
		Actor* ui = new Actor();
		ui->setPosition(Vector3(900.0f, -342.0f + i * step, 0.0f));
		SpriteComponent* sc = new SpriteComponent(ui, Assets::getTexture("SpeedUp"));
		sc->setVisible(false);

		UIUpSpeed.push_back(sc);
	}

	for (int i = 0; i < 20; i++)
	{
		Actor* ui = new Actor();
		ui->setPosition(Vector3(-342.0f + i * step, -475.0f, 0.0f));
		ui->setAngle(Vector3::unitZ, Maths::toRadians(90.0));
		SpriteComponent* sc = new SpriteComponent(ui, Assets::getTexture("SpeedRight"));
		sc->setVisible(false);

		UIRightSpeed.push_back(sc);
	}

	Actor* ui = new Actor();
	ui->setPosition(Vector3(-900.0f, 0.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(ui, Assets::getTexture("Spread"));

	ui = new Actor();
	ui->setPosition(Vector3(900.0f, 0.0f, 0.0f));
	sc = new SpriteComponent(ui, Assets::getTexture("Spread"));

	ui = new Actor();
	ui->setPosition(Vector3(0.0f, -475.0f, 0.0f));
	ui->setAngle(Vector3::unitZ, Maths::toRadians(90.0));
	sc = new SpriteComponent(ui, Assets::getTexture("Spread"));
}

Character::~Character()
{
	getGame().removeMovableActor(this);
}

void Character::actorInput(const struct InputState& inputState)
{
	//std::cout << getPosition().x << ", " << getPosition().y << std::endl;
	float angularSpeed = 0.0f;
	int uiSpeedIndex = 0;

	// ==================================================
	//					  MOVEMENTS
	// ==================================================
	// =======================================================================================================
	//												FORWARD
	// =======================================================================================================
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_W) == ButtonState::Pressed)
	{
		if (forwardSpeed >= 0)
			currentExpStepForward = initWExpStepForward;
		else
			currentExpStepForward = initSExpStepForward;
	}

	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
	{
		if (debugMovement)
			forwardSpeed += maxFowardSpeed;
		else
		{
			currentExpStepForward++;

			// ===== Move Forward =====
			if (!readyToMoveBack && forwardSpeed < maxFowardSpeed)
			{
				readyToMoveBack = false;
				if (exp(currentExpStepForward / stepForwardSpeed) > stepForwardSpeed)
				{
					forwardSpeed += stepForwardSpeed;
				}
				else
				{
					//std::cout << currentExpStepForward << std::endl;
					forwardSpeed += exp(currentExpStepForward / stepForwardSpeed);
				}
				
				if (forwardSpeed > maxFowardSpeed)
				{
					forwardSpeed = maxFowardSpeed;
				}

				// UI
				if (forwardSpeed > 0)
				{
					uiSpeedIndex = ((forwardSpeed / maxFowardSpeed) * 10);
					if(uiSpeedIndex < 10)
						UIForwardSpeed[5 + uiSpeedIndex]->setVisible(true);
				}
			}

			// ===== When stop and holding the key =====
			if (forwardSpeed == 0)
			{
				UIForwardSpeed[4]->setVisible(false);
			}

			// ===== Decelerate Move Back =====
			if (readyToMoveBack && forwardSpeed < 0)
			{

				if (exp(currentExpStepForward / stepForwardBreak) > stepForwardBreak)
				{
					forwardSpeed += stepForwardBreak;
				}
				else
				{
					forwardSpeed += exp(currentExpStepForward / stepForwardBreak);
				}

				if (forwardSpeed > 0)
				{
					forwardSpeed = 0;
				}

				// UI
				uiSpeedIndex = ((((forwardSpeed * -1) / maxNegatFowardSpeed) * 10) / 2) + 1;
				if (4 - uiSpeedIndex >= 0)
					UIForwardSpeed[4 - uiSpeedIndex]->setVisible(false);
			}
		}
	}

	if (inputState.keyboard.getKeyState(SDL_SCANCODE_W) == ButtonState::Released && forwardSpeed == 0)
	{
		readyToMoveBack = false;
	}


	// =======================================================================================================
	//											MOVE BACK
	// =======================================================================================================
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_S) == ButtonState::Pressed)
	{
		if (forwardSpeed > 0)
			currentExpStepForward = initSExpStepForward;
		else
			currentExpStepForward = initSExpNegStepForward;
	}
	
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
	{
		if (debugMovement)
			forwardSpeed -= maxNegatFowardSpeed;
		else
		{
			currentExpStepForward++;
			if (!readyToMoveBack && forwardSpeed > 0)
			{
				if (exp(currentExpStepForward / stepForwardBreak) > stepForwardBreak)
				{
					forwardSpeed -= stepForwardBreak;
				}
				else
				{
					forwardSpeed -= exp(currentExpStepForward / stepForwardBreak);
				}

				if (forwardSpeed < 0)
				{
					forwardSpeed = 0;
				}

				// UI
				uiSpeedIndex = ((forwardSpeed / maxFowardSpeed) * 10) + 1;
				if (uiSpeedIndex < 10)
					UIForwardSpeed[5 + uiSpeedIndex]->setVisible(false);
			}

			if (forwardSpeed == 0)
			{
				// UI
				UIForwardSpeed[5]->setVisible(false);
			}

			if (readyToMoveBack && forwardSpeed > -maxNegatFowardSpeed)
			{
				readyToMoveBack = true;

				if (exp(currentExpStepForward / stepNegateForwardSpeed) > stepNegateForwardSpeed)
				{
					forwardSpeed -= stepNegateForwardSpeed;
				}
				else
				{
					forwardSpeed -= exp(currentExpStepForward / stepNegateForwardSpeed);
				}

				if (forwardSpeed < -maxNegatFowardSpeed)
				{
					forwardSpeed = -maxNegatFowardSpeed;
				}

				// UI
				if (forwardSpeed < 0)
				{
					uiSpeedIndex = (((forwardSpeed * -1) / maxNegatFowardSpeed) * 10) / 2;
					if (4 - uiSpeedIndex >= 0)
						UIForwardSpeed[4 - uiSpeedIndex]->setVisible(true);
				}
			}
		}
	}

	if (inputState.keyboard.getKeyState(SDL_SCANCODE_S) == ButtonState::Released && forwardSpeed == 0)
	{
		readyToMoveBack = true;
	}


	// =======================================================================================================
	//											STRAFE UP
	// =======================================================================================================
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_SPACE) == ButtonState::Pressed)
	{
		canStabilized = false;
	}

	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_SPACE))
	{
		if (debugMovement)
			strafeUpSpeed += maxUpSpeed;
		else
		{
			if (strafeUpSpeed < maxUpSpeed)
			{
				if (strafeUpSpeed < 0)
					strafeUpSpeed += stepStrafeUpSpeed * 2;
				else
					strafeUpSpeed += stepStrafeUpSpeed;

				// UI
				if (uiSpeedIndexUp >= 0 && uiSpeedIndexUp < 10)
				{
					UIUpSpeed[10 + uiSpeedIndexUp]->setVisible(true);
				}
				if (uiSpeedIndexUp < 0 && uiSpeedIndexUp > -9)
				{
					UIUpSpeed[10 + uiSpeedIndexUp]->setVisible(false);
					UIUpSpeed[9 + uiSpeedIndexUp]->setVisible(false);
				}
			}
		}
	}

	if (inputState.keyboard.getKeyState(SDL_SCANCODE_SPACE) == ButtonState::Released
		&& inputState.keyboard.getKeyState(SDL_SCANCODE_LCTRL) == ButtonState::None)
	{
		canStabilized = true;
	}


	// =======================================================================================================
	//											STRAFE DOWN
	// =======================================================================================================
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_LCTRL) == ButtonState::Pressed)
	{
		canStabilized = false;
	}

	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_LCTRL))
	{
		if (debugMovement)
			strafeUpSpeed -= maxUpSpeed;
		else
		{
			if (strafeUpSpeed > -maxUpSpeed)
			{
				if (strafeUpSpeed > 0)
					strafeUpSpeed -= stepStrafeUpSpeed * 2;
				else
					strafeUpSpeed -= stepStrafeUpSpeed;

				// UI
				if (uiSpeedIndexUp >= 0 && uiSpeedIndexUp < 10)
				{
					UIUpSpeed[10 + uiSpeedIndexUp]->setVisible(false);
				}
				if (uiSpeedIndexUp < 0 && uiSpeedIndexUp > -10)
				{
					UIUpSpeed[10 + uiSpeedIndexUp]->setVisible(true);
				}
			}
		}
	}

	if (inputState.keyboard.getKeyState(SDL_SCANCODE_LCTRL) == ButtonState::Released
		&& inputState.keyboard.getKeyState(SDL_SCANCODE_SPACE) == ButtonState::None)
	{
		canStabilized = true;
	}


	// =======================================================================================================
	//											STRAFE RIGHT
	// =======================================================================================================
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_D) == ButtonState::Pressed)
	{
		canReturn = false;
	}
	
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		if (cameraComponent->getFPScam())
		{
			if (debugMovement)
				strafeRightSpeed += maxStrafeRightSpeed;
			else
			{
				if (strafeRightSpeed < maxStrafeRightSpeed)
				{
					if(strafeRightSpeed < 0)
						strafeRightSpeed += stepStrafeRightSpeed * 2;
					else
						strafeRightSpeed += stepStrafeRightSpeed;
				}

				// UI
				if (uiSpeedIndexRight >= 0 && uiSpeedIndexRight < 10)
				{
					UIRightSpeed[10 + uiSpeedIndexRight]->setVisible(true);
				}
				if (uiSpeedIndexRight < 0 && uiSpeedIndexRight > -9)
				{
					UIRightSpeed[10 + uiSpeedIndexRight]->setVisible(false);
					UIRightSpeed[9 + uiSpeedIndexRight]->setVisible(false);
				}
			}
		}
		else
			angularSpeed += sensitiveRota;
	}

	if (inputState.keyboard.getKeyState(SDL_SCANCODE_D) == ButtonState::Released
		&& inputState.keyboard.getKeyState(SDL_SCANCODE_A) == ButtonState::None)
	{
		canReturn = true;
	}

	// =======================================================================================================
	//											STRAFE LEFT
	// =======================================================================================================
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_A) == ButtonState::Pressed)
	{
		canReturn = false;
	}

	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
	{
		if (cameraComponent->getFPScam())
		{
			if (debugMovement)
				strafeRightSpeed -= maxStrafeRightSpeed;
			else
			{
				if (strafeRightSpeed > -maxStrafeRightSpeed)
				{
					if (strafeRightSpeed > 0)
						strafeRightSpeed -= stepStrafeRightSpeed * 2;
					else
						strafeRightSpeed -= stepStrafeRightSpeed;
				}

				// UI
				if (uiSpeedIndexRight >= 0 && uiSpeedIndexRight < 10)
				{
					UIRightSpeed[10 + uiSpeedIndexRight]->setVisible(false);
				}
				if (uiSpeedIndexRight < 0 && uiSpeedIndexRight > -10)
				{
					UIRightSpeed[10 + uiSpeedIndexRight]->setVisible(true);
				}
			}
		}
		else
			angularSpeed -= sensitiveRota;
	}

	if (inputState.keyboard.getKeyState(SDL_SCANCODE_A) == ButtonState::Released
		&& inputState.keyboard.getKeyState(SDL_SCANCODE_D) == ButtonState::None)
	{
		canReturn = true;
	}

	// =================================================================
	//						Display
	// =================================================================
	
	//std::cout << "Current Forward speed: " << forwardSpeed << std::endl;
	//std::cout << "Current Strafe Right speed: " << strafeRightSpeed << std::endl;
	//std::cout << "Current Strafe Up speed: " << strafeUpSpeed << std::endl;

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
	moveComponent->setUpSpeed(strafeUpSpeed);
	moveComponent->setAngularSpeed(angularSpeed);
	moveComponent->setStrafeSpeed(strafeRightSpeed);

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
}

void Character::updateActor(float dt)
{
	Actor::updateActor(dt);

	if (strafeUpSpeed > 0)
		uiSpeedIndexUp = (strafeUpSpeed / maxUpSpeed) * 10;
	else if (strafeUpSpeed < 0)
		uiSpeedIndexUp = floor((strafeUpSpeed / maxUpSpeed) * 10);
	else
		uiSpeedIndexUp = 0;

	if (strafeRightSpeed > 0)
		uiSpeedIndexRight = (strafeRightSpeed / maxStrafeRightSpeed) * 10;
	else if (strafeRightSpeed < 0)
		uiSpeedIndexRight = floor((strafeRightSpeed / maxStrafeRightSpeed) * 10);
	else
		uiSpeedIndexRight = 0;
	
	// Stabilized up movement
	if (canStabilized)
	{
		if (strafeUpSpeed > 0)
		{
			strafeUpSpeed -= stepStrafeUpSpeed;

			if (uiSpeedIndexUp < 10)
				UIUpSpeed[10 + uiSpeedIndexUp]->setVisible(false);

			if (strafeUpSpeed < 0)
				strafeUpSpeed = 0;
		}
		if (strafeUpSpeed < 0)
		{
			strafeUpSpeed += stepStrafeUpSpeed;

			if (uiSpeedIndexUp > -10)
				UIUpSpeed[9 + uiSpeedIndexUp]->setVisible(false);

			if (strafeUpSpeed > 0)
				strafeUpSpeed = 0;
		}

		if (strafeUpSpeed == 0)
		{
			UIUpSpeed[10]->setVisible(false);
			UIUpSpeed[9]->setVisible(false);
		}
	}

	// Stabilized right movement
	if (canReturn)
	{
		if (strafeRightSpeed > 0)
		{
			strafeRightSpeed -= stepStrafeRightSpeed;

			if (uiSpeedIndexRight < 10)
				UIRightSpeed[10 + uiSpeedIndexRight]->setVisible(false);

			if (strafeRightSpeed < 0)
				strafeRightSpeed = 0;
		}
		if (strafeRightSpeed < 0)
		{
			strafeRightSpeed += stepStrafeRightSpeed;

			if (uiSpeedIndexRight > -10)
				UIRightSpeed[9 + uiSpeedIndexRight]->setVisible(false);

			if (strafeRightSpeed > 0)
				strafeRightSpeed = 0;
		}

		if (strafeRightSpeed == 0)
		{
			UIRightSpeed[10]->setVisible(false);
			UIRightSpeed[9]->setVisible(false);
		}
	}

	//sphereX->setPosition(getPosition() + getForward() * 300);
	//sphereY->setPosition(getPosition() + getForward() * 300 + getRight() * 100);
	
	// Update position and rotation of model relatively to position
	Vector3 modelPosition = getPosition();

	fixCollisions();
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
