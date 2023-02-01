#pragma once
#include "Entity.h"
class UI_WindowBase {
public:
	virtual void Render();
	virtual void Update(float deltaTime);
};

class UI_TransformEditor : public UI_WindowBase
{
public: 
	void Render()override;
	void Update(float deltaTime)override;

private:
	Entity * selectedEntity;
	std::vector<Entity*> m_entities;
};