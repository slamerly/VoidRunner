#pragma once
#include "Actor.h"
#include "MeshComponent.h"

class Asteroid : public Actor
{
public:
	Asteroid();
	~Asteroid();

private:
	MeshComponent* mc;
};

