#pragma once
#include "GL_Dependencies.h"
#include "Transform.h"
#include <vector>
#include "Shader.h"

enum EntityType {
	DEFAULT_ENTITY,
	ENTITY_INSTANCE,
	WIREFRAME
};

class Entity
{
public:
	virtual void Update(float p_fDeltaTime);
	virtual void Render();

	virtual ~Entity();
	Entity(std::string p_name = "new GameObject");

	void SetName(std::string p_newName);
	std::string GetName();

	EntityType GetType();
	Shader * GetShader();

	Transform transform;
	bool m_enabled = true;

protected:
	std::string m_name;
	EntityType m_type = EntityType::DEFAULT_ENTITY;
	Shader * m_shader;
};
