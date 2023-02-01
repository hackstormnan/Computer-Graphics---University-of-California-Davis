#include "EntityManager.h"

void EntityManager::AddEntity(Entity* p_xEntity) {
	if (p_xEntity == nullptr) {
		return;
	}
	m_apxEntities.push_back(p_xEntity);
}

void EntityManager::RemoveEntity(Entity* p_xEntity) {
	if (p_xEntity == nullptr) {
		return;
	}
	for (unsigned int i = 0; i < m_apxEntities.size(); i++) {
		if (m_apxEntities[i] == p_xEntity) {
			m_apxEntities.erase(m_apxEntities.begin() + i);
			return;
		}
	}
}

void EntityManager::Clear() {
	for (unsigned int i = 0; i < m_apxEntities.size(); i++) {
		if (m_apxEntities[i] != nullptr) {
			delete m_apxEntities[i];
			m_apxEntities[i] = nullptr;
		}
	}
}

void EntityManager::Update(float p_fDeltaTime) {
	for (unsigned int i = 0; i < m_apxEntities.size(); i++) {
		if (m_apxEntities[i]->m_enabled == true)
		m_apxEntities[i]->Update(p_fDeltaTime);
	}
}

void EntityManager::Render() {
	for (unsigned int i = 0; i < m_apxEntities.size(); i++) {
		if(m_apxEntities[i]->m_enabled == true)
		m_apxEntities[i]->Render();
	}
}

int EntityManager::GetSize() {
	return (int)m_apxEntities.size();
}

std::vector<Entity*> EntityManager::GetAllEntities() {
	return m_apxEntities;
}

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

Entity* EntityManager::Find(std::string p_name)
{
	for (Entity* entity : m_apxEntities)
	{
		if (entity->GetName() == p_name)
		{
			return entity;
		}
	}

	return nullptr;
}