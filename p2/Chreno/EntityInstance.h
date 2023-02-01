#pragma once
#include "Entity.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class EntityInstance : public Entity
{
public:
	EntityInstance(std::string meshPath, std::string shaderPath);
	~EntityInstance();
	void Update(float deltaTime)override;
	void Render()override;

	Camera* camera;
private:

	std::vector<vertex> vertices;
	std::vector<unsigned int> indice;
};