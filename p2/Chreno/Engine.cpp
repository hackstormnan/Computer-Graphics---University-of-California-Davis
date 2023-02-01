#include "Engine.h"
#include "Renderer.h"
#include "Singleton.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "WireFrameEntity.h"
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

	float width  = (float)WindowProperty::width / 2;
	float height = (float)WindowProperty::height / 2;
	camera1->SetViewPort(0, height, width, height);
	camera2->SetViewPort(width, height, width, height);
	camera3->SetViewPort(0, 0, width, height);
	camera4->SetViewPort(width, 0, width, height);

	camera1->SetProjectionMode(ProjectionMode::PERSPECTIVE);
	camera1->transform.m_position += glm::vec3(0, 0, -3);

	camera2->SetProjectionMode(ProjectionMode::ORTHOGRAPHIC);

	camera3->SetProjectionMode(ProjectionMode::ORTHOGRAPHIC);
	camera3->transform.m_eulerAngles += glm::vec3(-90, 0, 0);

	camera4->SetProjectionMode(ProjectionMode::ORTHOGRAPHIC);
	camera4->transform.m_eulerAngles += glm::vec3(0, -90, 0);

	LoadObjects("Assets/Objs/Objects.txt");

	EntityManager::Get().GetAllEntities()[0]->m_enabled = true;
}

Engine::~Engine()
{
	SaveObjects("Assets/Objs/Objects.txt");
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

void Engine::LoadObjects(std::string filePath)
{
	std::stringstream ss;
	std::ifstream in_file(filePath);
	std::string line = "";
	int objNum = 0;
	if (!in_file.is_open()) {
		std::cout << filePath << " load failed!";

		return;
	}
	while (std::getline(in_file, line)) {
		ss.clear();
		ss.str(line);
		ss >> objNum;
		break;
	}

	for (int j = 0; j < objNum; j++)
	{
		int vertexNum = 0;
		int edgesNum = 0;
		std::vector<vertex> t_vertices;
		while (std::getline(in_file, line)) {
			ss.clear();
			ss.str(line);
			ss >> vertexNum;
			if (vertexNum > 0) {
				break;
			}
		}
		for (int i = 0; i < vertexNum; i++)
		{
			while (std::getline(in_file, line)) {
				ss.clear();
				ss.str(line);
				glm::vec3 temp_vec3;
				ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
				t_vertices.push_back(vertex{ temp_vec3 });
				break;
			}
		}
			
		std::vector<unsigned int> t_indicies;
		while (std::getline(in_file, line)) {
			ss.clear();
			ss.str(line);
			ss >> edgesNum;
			if (edgesNum > 0) {
				break;
			}
		}
		for (int i = 0; i < edgesNum; i++)
		{
			while (std::getline(in_file, line)) {
				ss.clear();
				ss.str(line);
				unsigned int start;
				unsigned int end;
				ss >> start >> end;
				t_indicies.push_back(start - 1);
				t_indicies.push_back(end - 1);
				break;
			}
		}
		WireFrameEntity * entity = new WireFrameEntity(t_vertices, t_indicies, "Assets/Shaders/Basic.shader");
		entity->SetName("object_" + std::to_string(j));
		entity->m_enabled = false;
		EntityManager::Get().AddEntity(entity); 
	}
}

void Engine::SaveObjects(std::string filePath)
{
	std::ofstream out(filePath);
	if (out.is_open())
	{
		std::vector<WireFrameEntity*> entities = EntityManager::Get().GetEntities<WireFrameEntity>();

		out << entities.size() << "\n";
		out << "\n";
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			std::vector<vertex> vertices = entities[i]->GetVertices();
			std::vector<unsigned int> indices = entities[i]->GetIndices();

			out << "\n";
			out << vertices.size() << "\n";

			for (unsigned int j = 0; j < vertices.size(); j++) {
				glm::vec4 pos = entities[i]->transform.GetWorldMatrix() * 
					glm::vec4(vertices[j].position.x, vertices[j].position.y, vertices[j].position.z, 1);

				out << pos.x << " " << pos.y << " " << pos.z << "\n";
			}

			out << "\n";
			out << indices.size() / 2 << "\n";

			for (int j = 0; j < indices.size(); j = j + 2) {
				out << indices[j] + 1 << " " << indices[j + 1] + 1 << "\n";
			}
		}

		out.close();
	}
}

void Engine::SetCamera(Camera* p_camera)
{
	std::vector<WireFrameEntity*> wireframes = EntityManager::Get().GetEntities<WireFrameEntity>();
	for (auto wireframe : wireframes) {
		wireframe->camera = p_camera;
	}
}