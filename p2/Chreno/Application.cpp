#include "Application.h"
#include "Engine.h"
#include "Renderer.h"
#include "InputManager.h"

Application::Application()
	:m_name("Cherno")
{
	m_framePerSec = 60.0f;
	m_timePerFrame = 1.0f / m_framePerSec;
}

bool Application::Init()
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WindowProperty::width, WindowProperty::height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return false;

	if (uiManager.Init(window) != true) {
		return false;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	engine = new Engine();

	return true;
}

void Application::Run()
{
	auto t_appStartTime = std::chrono::high_resolution_clock::now();
	
	while (GetRunningState())
	{
		auto currentTime = std::chrono::high_resolution_clock::now();

		std::chrono::duration<float> appDuration = (currentTime - t_appStartTime);
		m_appAccumulatedTime = appDuration.count();
		
		Renderer::Get().Clear();
		
		while (m_appAccumulatedTime >= m_targetFrameTime)
		{
			m_lastFrameTime = m_targetFrameTime;
			m_targetFrameTime = m_appAccumulatedTime + m_timePerFrame;
			float deltaTime = m_targetFrameTime - m_lastFrameTime;

			engine->Update(deltaTime);
			uiManager.Update(deltaTime);
		}

		//glViewport(0, 0, WindowProperty::width /2 , WindowProperty::height / 2);
		
		glfwPollEvents();

		engine->Render();
		uiManager.Render();

		glfwSwapBuffers(window);

		InputManager::Get().InputHandle(window);
	}
}

void Application::ShutDown()
{
	uiManager.ShutDown();
	delete engine;
	glfwTerminate();
}

bool Application::GetRunningState()
{
	if (glfwWindowShouldClose(window) == 1) {
		return false;
	 }

	return true;
}