#pragma once
#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(Actor* owner);
	virtual ~MeshComponent();

	bool getVisible() const { return isVisible; }
	void setVisible(bool isVisibleP);

	virtual void draw(class Shader& shader);
	virtual void setMesh(class Mesh& meshP);
	void setTextureIndex(size_t textureIndexP);

	bool getLighted() const { return isLighted; }
	void setLighted(bool lighted);


protected:
	Mesh* mesh;
	size_t textureIndex;
	bool isVisible;
	bool isLighted;
};

