#pragma once
#include "GL_Dependencies.h"

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
	void GetBiziers(std::vector<std::vector<glm::vec2>>& p_biziers);
	void GetBspines(std::vector<std::vector<glm::vec2>>& p_bspines);
private:
	std::vector<std::vector<glm::vec2>> biziers;
	int selectedBizer = -1;
	std::vector<std::vector<glm::vec2>> bspines;
	int selectedBspine = -1;
	bool onLevel_1 = true;

	bool SecondaryOn();
	void HandleBezier();
	bool HandleBezierPoints(float w, float h);
	void HandleBspine();
	bool HandleBspinePoints(float w, float h);
};