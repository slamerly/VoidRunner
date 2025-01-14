#include "Station.h"
#include "Assets.h"

Station::Station(int meshNumber)
{
	mc = new MeshComponent(this);
	
	Quaternion rotationx = Quaternion(Vector3::unitX, Maths::toRadians(90));
	Quaternion rotationy = Quaternion(Vector3::unitY, Maths::toRadians(25));
	Quaternion rotationz = Quaternion(Vector3::unitZ, Maths::toRadians(45));
	
	setScale(10);

	switch (meshNumber)
	{
	case 1:
		mc->setMesh(Assets::getMesh("Station01"));
		break;

	case 2:
		mc->setMesh(Assets::getMesh("Station03_Base"));
		MeshComponent* mc1;
		mc1 = new MeshComponent(this);
		mc1->setMesh(Assets::getMesh("Station03_Dock"));
		MeshComponent* mc2;
		mc2 = new MeshComponent(this);
		mc2->setMesh(Assets::getMesh("Station03_Ring"));

		rotationx = Quaternion(Vector3::unitX, Maths::toRadians(-90));
		rotationy = Quaternion(Vector3::unitY, Maths::toRadians(-25));
		rotationz = Quaternion(Vector3::unitZ, Maths::toRadians(-45));
		
		break;

	default:
		break;
	}
	
	setRotation(Quaternion::concatenate(rotationx, rotationy));
	Quaternion stationRotation = Quaternion::concatenate(getRotation(), rotationz);
	setRotation(stationRotation);
}

Station::~Station()
{
}
