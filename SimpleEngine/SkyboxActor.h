#pragma once
#include "Actor.h"
#include "MeshComponent.h"

class SkyboxActor : public Actor
{
public:
	SkyboxActor();

	MeshComponent* getMeshComponent() { return mc; }

private:
	MeshComponent* mc;
};

