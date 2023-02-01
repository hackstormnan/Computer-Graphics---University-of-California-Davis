#pragma once
#include "Entity.h"
#include <string>
#include "Camera.h"
class WireFrameEntity : public Entity
{
public:
	WireFrameEntity(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::string filePath);
	void Render()override;
	void Update(float deltaTime)override;

	Camera* camera;

	std::vector<vertex> GetVertices();
	std::vector<unsigned int> GetIndices();
private:
	std::string m_filePath;

	int m_count;

	VertexArray* vao;
	VertexBuffer* vbo;
	IndexBuffer* ibo;

	VertexBufferLayout m_layout;

	std::vector<vertex> m_vertices;
	std::vector<unsigned int> m_indices;
};