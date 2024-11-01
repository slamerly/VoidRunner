#include "Asteroid.h"
#include "Assets.h"

Asteroid::Asteroid()
{
	//srand(time(NULL));
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


	//srand(time(NULL));
	int selectScale = 1;
	selectScale = rand() % 10 + 1;

	setScale(selectScale);
	std::cout << selectScale << std::endl;
}

Asteroid::~Asteroid()
{
}
