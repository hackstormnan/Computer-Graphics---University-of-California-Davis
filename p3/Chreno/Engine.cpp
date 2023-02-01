#include "Engine.h"
#include "Renderer.h"
#include "Singleton.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "Application.h"

Engine::Engine()
{
	camera1 = new Camera();
	camera2 = new Camera();
	camera3 = new Camera();
	camera4 = new Camera();
	camera1->transform.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	camera2->transform.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	camera3->transform.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	camera4->transform.m_position = glm::vec3(0.0f, 0.0f, 0.0f);

	float width = (float)WindowProperty::width / 2;
	float height = (float)WindowProperty::height / 2;
	camera1->SetViewPort(0, height, width, height);
	camera2->SetViewPort(width, height, width, height);
	camera3->SetViewPort(0, 0, width, height);
	camera4->SetViewPort(width, 0, width, height);

	camera1->SetProjectionMode(ProjectionMode::PERSPECTIVE);
	camera1->transform.m_position += glm::vec3(0, 0, -3);

	camera2->SetProjectionMode(ProjectionMode::ORTHOGRAPHIC);
	camera2->transform.m_position -= camera2->transform.GetForward() * 3.0f;

	camera3->SetProjectionMode(ProjectionMode::ORTHOGRAPHIC);
	camera3->transform.m_eulerAngles += glm::vec3(-90, 0, 0);
	camera3->transform.m_position -= camera3->transform.GetForward() * 3.0f;

	camera4->SetProjectionMode(ProjectionMode::ORTHOGRAPHIC);
	camera4->transform.m_eulerAngles += glm::vec3(0, -90, 0);
	camera4->transform.m_position -= camera4->transform.GetForward() * 3.0f;

	Entity * e1 = new EntityInstance("Assets/Objs/Sphere.obj", "Assets/Shaders/Basic.shader", "Assets/Textures/Blue.jpg");
	e1->transform.m_eulerAngles = glm::vec3(20, 35, 0);
	Entity * e2 = new EntityInstance("Assets/Objs/Cube.obj", "Assets/Shaders/Basic.shader", "Assets/Textures/DefaultTexture.jpg", DrawType::PAINTERS_ALGORITHEM);
	Entity * e3 = new EntityInstance("Assets/Objs/Cylinder.obj", "Assets/Shaders/Basic.shader", "Assets/Textures/Green.jpg");
	
	e1->SetName("Sphere");
	e2->SetName("Cube");
	e3->SetName("Cylinder");
	
	e1->m_enabled = false;
	e1->transform.m_scale = glm::vec3(1.0f);
	e3->m_enabled = false;
	e3->transform.m_scale = glm::vec3(0.8f);
	
	EntityManager::Get().AddEntity(e1);
	EntityManager::Get().AddEntity(e2);
	EntityManager::Get().AddEntity(e3);
}

Engine::~Engine()
{

}

void Engine::Update(float deltaTime)
{
	camera1->Update(deltaTime);
	camera2->Update(deltaTime);
	camera3->Update(deltaTime);
	camera4->Update(deltaTime);
	EntityManager::Get().Update(deltaTime);
}

void Engine::Render()
{
	camera1->Render();
	SetCamera(camera1);
	EntityManager::Get().Render();

	camera2->Render();
	SetCamera(camera2);
	EntityManager::Get().Render();

	camera3->Render();
	SetCamera(camera3);
	EntityManager::Get().Render();

	camera4->Render();
	SetCamera(camera4);
	EntityManager::Get().Render();
}

void Engine::SetCamera(Camera* p_camera)
{
	std::vector<EntityInstance*> wireframes = EntityManager::Get().GetEntities<EntityInstance>();
	for (auto wireframe : wireframes) {
		wireframe->camera = p_camera;
	}
}