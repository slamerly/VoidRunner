#include "Asteroid.h"
#include "Assets.h"

Asteroid::Asteroid()
{
	mc = new MeshComponent(this);

	int selectMesh = 0;

	selectMesh = rand() % 4;

	switch (selectMesh)
	{
	case 0:
		mc->setMesh(Assets::getMesh("Rock_Small01"));
		break;
	case 1:
		mc->setMesh(Assets::getMesh("Rock_Small02"));
		break;
	case 2:
		mc->setMesh(Assets::getMesh("Rock_Medium01"));
		break;
	case 3:
		mc->setMesh(Assets::getMesh("Rock_Medium02"));
		break;
	default:
		mc->setMesh(Assets::getMesh("Rock_Small01"));
		break;
	}

	int selectScale = 1;
	selectScale = rand() % 10 + 1;

	setScale(selectScale);
	std::cout << selectScale << std::endl;
}

Asteroid::Asteroid(AsteroidSize size)
{
	mc = new MeshComponent(this);
	
	// ===== Size test =====
	//sphere = new SphereActor();
	//sphere->getMeshComponent()->setTextureIndex(4);
	int sphereSize = 1;
	

	int selectMesh = 0;
	int selectScale = 1;

	// ===================================================================
	//							Mesh & Size
	// ===================================================================

	switch (size)
	{
	case SMALL:
		selectMesh = rand() % 2;
		selectScale = rand() % 10 + 1;

		if (selectMesh == 0)
		{
			mc->setMesh(Assets::getMesh("Rock_Small01"));
		}
		else
		{
			mc->setMesh(Assets::getMesh("Rock_Small02"));
		}

		sphereSize = 65;
		asteroidSize = 1625;

		//sphere->setScale(sphereSize);

		setScale(selectScale);
		break;

	case MEDIUM:
		selectMesh = rand() % 4;

		switch (selectMesh)
		{
		case 0:
			mc->setMesh(Assets::getMesh("Rock_Small01"));
			selectScale = rand() % 11 + 20;
			break;
		case 1:
			mc->setMesh(Assets::getMesh("Rock_Small02"));
			selectScale = rand() % 11 + 20;
			break;
		case 2:
			mc->setMesh(Assets::getMesh("Rock_Medium01"));
			selectScale = rand() % 6 + 4;
			break;
		case 3:
			mc->setMesh(Assets::getMesh("Rock_Medium02"));
			selectScale = rand() % 6 + 4;
			break;
		default:
			break;
		}

		sphereSize = 190;
		asteroidSize = 4750;

		setScale(selectScale);
		break;

	case LARGE:
		selectMesh = rand() % 2;
		selectScale = rand() % 6 + 25;

		if (selectMesh == 0)
		{
			mc->setMesh(Assets::getMesh("Rock_Medium01"));
		}
		else
		{
			mc->setMesh(Assets::getMesh("Rock_Medium02"));
		}

		sphereSize = 595;
		asteroidSize = 14875;

		setScale(selectScale);
		break;
	case NONE:
		std::cout << "NONE" << std::endl;

		break;
	default:
		break;
	}

	// ================================================================
	//							Rotation
	// ================================================================
	int rotX = 0;
	int rotY = 0;
	int rotZ = 0;

	rotX = rand() % 361;
	setAngle(Vector3::unitX, rotX);
	rotY = rand() % 361;
	setAngle(Vector3::unitY, rotY);
	rotZ = rand() % 361;
	setAngle(Vector3::unitZ, rotZ);
}

Asteroid::~Asteroid()
{
	//sphere->setState(Actor::ActorState::Dead);
}

void Asteroid::setPosition(Vector3 positionP)
{
	Actor::setPosition(positionP);
	//sphere->setPosition(positionP);
}
