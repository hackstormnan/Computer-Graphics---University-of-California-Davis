#pragma once
#include "GL_Dependencies.h"
#include "EntityInstance.h"
#include "Camera.h"
#include "Singleton.h"

class Engine : public Singleton<Engine>
{
	friend class Singleton<Engine>;
public:
	Engine();
	~Engine();
	void Update(float deltaTime);
	void Render();

	bool m_halfToneOn = false;
private:
	Camera * camera1;
	Camera * camera2;
	Camera * camera3;
	Camera * camera4;

	void SetCamera(Camera * p_camera);

};