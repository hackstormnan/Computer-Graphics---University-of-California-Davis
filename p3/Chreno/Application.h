#pragma once
#include <string>
#include "GL_Dependencies.h"
#include <chrono>
#include "UIManager.h"
class Engine;

struct WindowProperty
{
	static const int width = 1920;
	static const int height = 1080;
};

class Application {
public:
	Application();

	bool Init();
	void Run();
	void ShutDown();
	std::string m_name;

	GLFWwindow * window;
	Engine * engine;
	UIManager uiManager;
	
	float m_framePerSec;
	float m_timePerFrame;
	float m_targetFrameTime;
	float m_lastFrameTime;
	float m_appAccumulatedTime;

	bool GetRunningState();
};