#pragma once
#include "GL_Dependencies.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Entity.h"

struct vertex {
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

struct Face {
	Face(int p1, int p2, int p3) {
		m_indicies.push_back(p1);
		m_indicies.push_back(p2);
		m_indicies.push_back(p3);
	}
	std::vector<unsigned int> m_indicies; //size of 3
};

class Camera;
class Mesh
{
public:
	Mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, VertexBufferLayout layout, Entity * m_parent = nullptr);
	Mesh(std::string filePath, VertexBufferLayout layout, Entity * m_parent = nullptr);
	void Bind()const;
	void BindFace(int index)const;
	void SortFaces(Camera* cam)const;

	void UnBind()const;

	int GetCount()const;
	int GetFaceCount()const;
	float GetMinDepth(Face p_face, Camera * cam)const;
	float CalculateDepth(glm::vec3 p_face, Camera * cam)const;
	~Mesh();

	std::string m_filePath;
private:
	void LoadOBJ(std::string filePath);

	Entity * m_parent;

	VertexArray *vao;
	VertexBuffer *vbo;
	IndexBuffer * ibo;

	VertexBufferLayout m_layout;

	int m_count;
	int m_FaceCount;

	std::vector<vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	mutable std::vector<Face> m_faces;

	void GenVerticies();
};