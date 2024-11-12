#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "SphereActor.h"

enum AsteroidSize
{
	SMALL,
	MEDIUM,
	LARGE,
	NONE
};


class Asteroid : public Actor
{
public:
	Asteroid();
	Asteroid(AsteroidSize size);
	~Asteroid();

	SphereActor* sphere;

private:
	MeshComponent* mc;
	
};

