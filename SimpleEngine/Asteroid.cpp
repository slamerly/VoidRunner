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

	
	// Size test
	//sphere = new SphereActor();
	//sphere->getMeshComponent()->setTextureIndex(4);
	

	int selectMesh = 0;
	int selectScale = 1;

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

		asteroidSize = 65;

		setScale(selectScale);
		break;

	case MEDIUM:
		selectMesh = rand() % 4;
		selectScale = 1;

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
			selectScale = rand() % 6 + 2;
			break;
		case 3:
			mc->setMesh(Assets::getMesh("Rock_Medium02"));
			selectScale = rand() % 6 + 2;
			break;
		default:
			break;
		}

		asteroidSize = 145;

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

		asteroidSize = 595;

		//sphere->setScale(asteroidSize);

		setScale(selectScale);
		break;
	case NONE:
		std::cout << "NONE" << std::endl;

		break;
	default:
		break;
	}
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
