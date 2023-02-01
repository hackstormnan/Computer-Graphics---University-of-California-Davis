#include "Entity.h"
#include "Singleton.h"
#include "Transform.h"

void Entity::Update(float p_fDeltaTime)
{
}

void Entity::Render()
{
}

Entity::Entity(std::string p_name)
{
	m_name = p_name;
	m_enabled = true;
}

Entity::~Entity()
{
}

std::string Entity::GetName()
{
	return m_name;
}

EntityType Entity::GetType()
{
	return m_type;
}

Mesh * Entity::GetMesh()
{
	return m_mesh;
}

Shader * Entity::GetShader()
{
	return m_shader;
}

void Entity::SetName(std::string p_newName)
{
	m_name = p_newName;
}