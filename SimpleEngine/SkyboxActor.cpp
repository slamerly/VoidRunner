#include "SkyboxActor.h"
#include "MeshComponent.h"
#include "Assets.h"

SkyboxActor::SkyboxActor()
{
	mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Skybox"));
}