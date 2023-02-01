#include "WireFrameEntity.h"
#include "Renderer.h"

/*m_vertices = {
	{{0.0f, 0.0f, 0.0f}},
	{{1.0f, 0.0f, 0.0f}},
	{{0.0f, 1.0f, 0.0f}},
	{{0.0f, 0.0f, 1.0f}}
};
m_indices = {
	1, 2,
	1, 3,
	1, 4,
	2, 3,
	2, 4,
	3, 4,
};*/
WireFrameEntity::WireFrameEntity(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::string shaderPath)
{
	m_vertices = vertices;
	m_indices = indices;
	m_filePath = shaderPath;
		
	m_layout.Push<float>(3);

	m_shader = new Shader("Assets/Shaders/Basic.shader");
	m_count = (int)m_indices.size();

	vbo = new VertexBuffer(&m_vertices[0], sizeof(vertex) * (int)m_vertices.size());
	vao = new VertexArray();
	vao->AddBuffer(vbo, m_layout);
	ibo = new IndexBuffer(&m_indices[0], m_count);

	vbo->UnBind();
	vao->UnBind();
	ibo->UnBind();
	m_type = WIREFRAME;
}

static float accumulator = 0.0f;
static float increament = 0.05f;

void WireFrameEntity::Update(float deltaTime)
{
	accumulator += increament * deltaTime;
	if (accumulator >= 1)
		increament = -0.05f;
	else if (accumulator <= 0)
		increament = 0.05f;
}

std::vector<vertex> WireFrameEntity::GetVertices()
{
	return m_vertices;
}

std::vector<unsigned int> WireFrameEntity::GetIndices()
{
	return m_indices;
}

void WireFrameEntity::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_shader->Bind();
	m_shader->SetUniform4f("u_color", accumulator, 0.3f, 0.8f, 1.0f);
	m_shader->SetUniformMat4x4("u_world", transform.GetWorldMatrix());
	m_shader->SetUniformMat4x4("u_view", camera->GetViewMatrix());
	m_shader->SetUniformMat4x4("u_projection", camera->GetProjectionMatrix());

	Renderer::Get().Draw(*vao, *ibo ,*m_shader, DrawMode::LINE);
}