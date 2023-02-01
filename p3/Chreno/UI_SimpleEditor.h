#pragma once
#include "Entity.h"
class UI_WindowBase {
public:
	virtual void Render();
	virtual void Update(float deltaTime);
};

class UI_SimpleEditor : public UI_WindowBase
{
public: 
	void Render()override;
	void Update(float deltaTime)override;
private:
	Entity * selectedEntity = nullptr;
	bool m_usingPhong = true;
	bool m_usingPhongLastFrame = true;

	bool m_usingPainter = false;
	bool m_usingPainterLastFrame = false;
	std::vector<Entity*> m_entities;
};
