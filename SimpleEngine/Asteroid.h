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

	// Size test
	SphereActor* sphere;

	const float getAsteroidSize() { return asteroidSize; }

	void setPosition(Vector3 positionP);

private:
	MeshComponent* mc;
	float asteroidSize = 0;
	
};

