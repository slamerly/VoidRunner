#pragma once
#include "Actor.h"
#include "MeshComponent.h"

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

private:
	MeshComponent* mc;
};

