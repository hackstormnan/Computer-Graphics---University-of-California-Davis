#include "Mesh.h"
#include "Singleton.h"
#include "Camera.h"
#include <iostream>

//Obj loader reference: https://www.youtube.com/watch?v=zMwKYCVNcn0&t=236s
void Mesh::LoadOBJ(std::string filePath) 
{
	std::vector<glm::vec3> vertex_position;
	std::vector<glm::vec2> vertex_texcoord;
	std::vector<glm::vec3> vertex_normal;

	std::vector<unsigned int> vertex_position_indicies;
	std::vector<unsigned int> vertex_texcoord_indicies;
	std::vector<unsigned int> vertex_normal_indicies;

	std::stringstream ss;
	std::ifstream in_file(filePath);
	std::string line = "";
	std::string prefix = "";

	glm::vec3 temp_vec3;
	glm::vec3 temp_vec2;
	unsigned int temp_glint = 0;

	if (!in_file.is_open()) {
		std::cout << filePath << " load failed!";
		
		return;
	}
	while (std::getline(in_file, line)) {
		prefix = "";

		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "v") { //vertex position
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_position.push_back(temp_vec3);
		}
		else if (prefix == "vt") { //texcoord
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoord.push_back(temp_vec2);
		}
		else if (prefix == "vn") { //normal
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normal.push_back(temp_vec3);
		}
		else if (prefix == "f") { //faces
			int counter = 0;
			while (ss >> temp_glint) {
				if (counter == 0) {
					//position
					vertex_position_indicies.push_back(temp_glint);
				}
				else if (counter == 1) {
					//texcoord
					vertex_texcoord_indicies.push_back(temp_glint);
				}
				else if (counter == 2) {
					//normal
					vertex_normal_indicies.push_back(temp_glint);
				}
				//process charactors
				if (ss.peek() == '/') {
					counter++;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					counter = 0;
					ss.ignore(1, ' ');
				}
				if (counter > 2) {
					counter = 0;
				}
			}
		}
		else if (prefix == "#") {

		}
		else if (prefix == "use_mtl") {

		}
		else if (prefix == "o") {

		}
		else if (prefix == "s") {

		}
		else {

		}
	}
	//build vertex array
	m_vertices.resize(vertex_position_indicies.size(), vertex());
	m_indices.resize(vertex_position_indicies.size());

	//Load indices
	for (unsigned int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].position = vertex_position[vertex_position_indicies[i] - 1];
		m_vertices[i].texcoord = vertex_texcoord[vertex_texcoord_indicies[i] - 1];
		m_vertices[i].normal = vertex_normal[vertex_normal_indicies[i] - 1];
		m_indices[i]= i;
	}

	m_filePath = filePath;
}

void Mesh::GenVerticies()
{
	m_count = (int)m_indices.size();
	m_FaceCount = m_count / 3;

	for (int i = 0; i < m_FaceCount; i++)
	{
		m_faces.push_back(Face(m_indices[0 + i * 3], m_indices[1 + i * 3], m_indices[2 + i * 3]));
	}

	vbo = new VertexBuffer(&m_vertices[0], sizeof(vertex) * (int)m_vertices.size());
	vao = new VertexArray();
	vao->AddBuffer(vbo, m_layout);
	ibo = new IndexBuffer(&m_indices[0], m_count);

	vbo->UnBind();
	vao->UnBind();
	ibo->UnBind();
}

Mesh::Mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, VertexBufferLayout layout, Entity* p_parent) {
	m_vertices = vertices;
	m_indices = indices;
	m_layout = layout;
	m_parent = p_parent;

	GenVerticies();
}

Mesh::Mesh(std::string filePath, VertexBufferLayout layout, Entity * p_parent)
{
	LoadOBJ(filePath);

	m_layout = layout;
	m_parent = p_parent;

	GenVerticies();
}

void Mesh::Bind()const
{
	ibo->UpdateBuffer(&m_indices[0], m_count);
	vao->Bind();
	ibo->Bind();
}

void Mesh::UnBind() const
{
	vao->UnBind();
	ibo->UnBind();
}

int Mesh::GetCount() const
{
	return m_count;
}

int Mesh::GetFaceCount() const
{
	return m_FaceCount;
}

float Mesh::GetMinDepth(Face p_face, Camera * cam) const
{
	float minDis = CalculateDepth(m_vertices[p_face.m_indicies[0]].position, cam);
	for (unsigned int i = 1; i < p_face.m_indicies.size(); i++)
	{
		float cur = CalculateDepth(m_vertices[p_face.m_indicies[i]].position, cam) < minDis;
		if (cur < minDis) {
			minDis = cur;
		}
	}
	return minDis;
}

float Mesh::CalculateDepth(glm::vec3 p_pos, Camera * cam) const
{
	glm::vec4 pos = glm::vec4(p_pos.x, p_pos.y, p_pos.z, 0);
	glm::vec3 depth = pos * m_parent->transform.GetWorldMatrix();

	float distance = glm::distance(depth, cam->transform.m_position);
	return distance;
}

void Mesh::BindFace(int index)const
{
	ibo->UpdateBuffer(&m_faces[index].m_indicies[0], 3);
}

void Mesh::SortFaces(Camera * cam)const 
{
	for (int i = 0; i < m_FaceCount; i++)
	{
		for (int j = i + 1; j < m_FaceCount; j++)
		{
			float depth1 = GetMinDepth(m_faces[i], cam);
			float depth2 = GetMinDepth(m_faces[j], cam);
			if (depth1 < depth2) {
				Face temp = m_faces[i];
				m_faces[i] = m_faces[j];
				m_faces[j] = temp;
			}
		}
	}
}

Mesh::~Mesh()
{
	delete vbo;
	delete ibo;
	delete vao;
}