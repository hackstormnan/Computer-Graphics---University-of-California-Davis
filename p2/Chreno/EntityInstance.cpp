#include "EntityInstance.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

EntityInstance::EntityInstance(std::string meshPath, std::string shaderPath)
{
	m_type = EntityType::ENTITY_INSTANCE;

	VertexBufferLayout layout;
	layout.Push<float>(3);

	m_shader = new Shader(shaderPath);
	m_mesh = new Mesh(meshPath, layout);

	m_shader->UnBind();
	m_mesh->UnBind();
}

EntityInstance::~EntityInstance()
{
	delete m_mesh;
	delete m_shader;
}

static float accumulator = 0.0f;
static float increament = 0.05f;

void EntityInstance::Update(float deltaTime)
{
	accumulator += increament * deltaTime;
	if (accumulator >= 1)
		increament = -0.05f;
	else if (accumulator <= 0)
		increament = 0.05f;
}

void EntityInstance::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_shader->Bind();
	m_shader->SetUniform4f("u_color", accumulator, 0.3f, 0.8f, 1.0f);
	m_shader->SetUniformMat4x4("u_world", transform.GetWorldMatrix());
	m_shader->SetUniformMat4x4("u_view", camera->GetViewMatrix());
	m_shader->SetUniformMat4x4("u_projection", camera->GetProjectionMatrix());
	Renderer::Get().Draw(* m_mesh, * m_shader);
}