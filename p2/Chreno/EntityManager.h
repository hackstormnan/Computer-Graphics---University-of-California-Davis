#pragma once
#include <vector>
#include <string>
#include "Entity.h"
#include "Singleton.h"

class EntityManager : public Singleton<EntityManager>
{
	friend class Singleton<EntityManager>;
public:
	void AddEntity(Entity* p_xEntity);
	void RemoveEntity(Entity* p_xEntity);
	void Clear();
	void Update(float p_fDeltaTime);
	void Render();
	int  GetSize();
	std::vector<Entity*> GetAllEntities();
	template<typename T>
	std::vector<T*> GetEntities();
	EntityManager();
	~EntityManager();

	Entity* Find(std::string p_name);
private:
	std::vector<Entity*> m_apxEntities;
};

template<typename T>
inline std::vector<T*> EntityManager::GetEntities() {
	std::vector<T*> result;
	for (Entity* entity : m_apxEntities) {
		T* temp = dynamic_cast<T*>(entity);
		if (temp != nullptr) {
			result.push_back(temp);
		}
	}
	return result;
}
