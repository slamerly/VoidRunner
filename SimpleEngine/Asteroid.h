#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "SphereActor.h"
#include "MoveComponent.h"

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
	void isRotation();

private:
	MeshComponent* mc;
	MoveComponent* mvc;
	float asteroidSize = 0;

	bool rotate = false;
	float speed = 0;
};

