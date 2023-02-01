#pragma once
#include "GL_Dependencies.h"
#include "WireFrameEntity.h"
#include "EntityInstance.h"
#include "Camera.h"

class Engine
{
public:
	Engine();
	~Engine();
	void Update(float deltaTime);
	void Render();

	void LoadObjects(std::string filePath);
	void SaveObjects(std::string filePath);
private:
	Camera * camera1;
	Camera * camera2;
	Camera * camera3;
	Camera * camera4;
	void SetCamera(Camera * p_camera);
};