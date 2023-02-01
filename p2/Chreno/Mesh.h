#pragma once
#include "GL_Dependencies.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

struct vertex {
	glm::vec3 position;
};

class Mesh
{
public:
	Mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, VertexBufferLayout layout);
	Mesh(std::string filePath, VertexBufferLayout layout);
	void Bind()const;
	void UnBind()const;
	int GetCount()const;
	~Mesh();

	std::string m_filePath;
private:
	void LoadOBJ(std::string filePath);

	VertexArray *vao;
	VertexBuffer *vbo;
	IndexBuffer * ibo;

	VertexBufferLayout m_layout;

	int m_count;

	std::vector<vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	void GenVerticies();
};