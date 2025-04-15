#include "TargetPoint.h"
#include "Assets.h"

TargetPoint::TargetPoint()
{
	sphere = new SphereActor();
	sphere->setScale(100.0f);
	sphere->getMeshComponent()->setTextureIndex(1);
	sphere->setPosition(this->getPosition());
}

TargetPoint::~TargetPoint()
{
}

void TargetPoint::setNewLocation()
{
	sphere->setPosition(this->getPosition());
}
