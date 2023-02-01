#include "EntityInstance.h"
#include "Renderer.h"

EntityInstance::EntityInstance(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::string shaderPath, std::string texPath, DrawType p_drawType)
{
	m_vertices = vertices;
	m_indices = indices;

	m_shaderPath = shaderPath;
	m_texPath = texPath;

	m_layout.Push<float>(3);
	m_layout.Push<float>(2);
	m_layout.Push<float>(3);

	m_texture = new Texture(m_texPath);
	m_shader = new Shader(m_shaderPath);
	m_mesh = new Mesh(vertices, indices, m_layout, this);

	m_drawType = p_drawType;
	m_type = WIREFRAME;
}

EntityInstance::EntityInstance(std::string meshPath, std::string shaderPath, std::string texPath, DrawType p_drawType)
{
	m_meshPath = meshPath;
	m_shaderPath = shaderPath;
	m_texPath = texPath;

	m_layout.Push<float>(3);
	m_layout.Push<float>(2);
	m_layout.Push<float>(3);

	m_texture = new Texture(m_texPath);
	m_shader = new Shader(m_shaderPath);
	m_mesh = new Mesh(m_meshPath, m_layout, this);

	m_drawType = p_drawType;
	m_type = WIREFRAME;
}

void EntityInstance::Update(float deltaTime)
{
}

std::vector<vertex> EntityInstance::GetVertices()
{
	return m_vertices;
}

std::vector<unsigned int> EntityInstance::GetIndices()
{
	return m_indices;
}

void EntityInstance::UpdateShader(std::string p_path)
{
	delete m_shader;
	m_shader = new Shader(p_path);
}

void EntityInstance::UpdateDrawType(DrawType p_drawType)
{
	m_drawType = p_drawType;
}

void EntityInstance::Render()
{
	glEnable(GL_CULL_FACE);
	m_mesh->Bind();
	m_texture->Bind();
	m_shader->Bind();
	m_shader->SetUniform1i("u_texture", 0);
	
	m_shader->SetUniformVec3("light.ambient", glm::vec3(0.2f));
	m_shader->SetUniformVec3("light.diffuse", glm::vec3(0.8f));
	m_shader->SetUniformVec3("light.direction", glm::vec3(0.0f, 0.0f, 1.0f));
	m_shader->SetUniformVec3("u_camPos", camera->transform.m_position);

	m_shader->SetUniformMat4x4("u_world", transform.GetWorldMatrix());
	m_shader->SetUniformMat4x4("u_view", camera->GetViewMatrix());
	m_shader->SetUniformMat4x4("u_projection", camera->GetProjectionMatrix());

	if (m_drawType == DEFAULT_ENTITY) {
		Renderer::Get().Draw(*m_mesh, *m_shader, DrawMode::TRIANGLE);
	}
	else if (m_drawType == PAINTERS_ALGORITHEM) {
		Renderer::Get().DrawByFaces(*m_mesh, *m_shader, camera, DrawMode::TRIANGLE);
	}

	m_mesh->UnBind();
	m_texture->UnBind();
	m_shader->UnBind();
}