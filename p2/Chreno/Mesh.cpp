#include "Mesh.h"
#include "Singleton.h"

void Mesh::LoadOBJ(std::string filePath) 
{
	std::stringstream ss;
	std::ifstream in_file(filePath);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	GLint temp_glint = 0;

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
			m_vertices.push_back(vertex{ temp_vec3 });
		}
		else if (prefix == "vt") { //texcoord

		}
		else if (prefix == "vn") { //normal

		}
		else if (prefix == "f") { //faces
			int counter = 0;
			while (ss >> temp_glint) {
				//push back indicies
				if (counter == 0) {
					m_indices.push_back(temp_glint - 1);
				}
				else if (counter == 1) {
					//texcoord
				}
				else if (counter == 2) {
					//normal
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
	m_filePath = filePath;

	std::cout << m_filePath << " Number of vertices: " << m_vertices.size() << "\n";
	std::cout << m_filePath << " Number of indices: " << m_indices.size() << "\n";
}

void Mesh::GenVerticies()
{
	m_count = (int)m_indices.size();

	vbo = new VertexBuffer(&m_vertices[0], sizeof(vertex) * (int)m_vertices.size());
	vao = new VertexArray();
	vao->AddBuffer(vbo, m_layout);
	ibo = new IndexBuffer(&m_indices[0], m_count);

	vbo->UnBind();
	vao->UnBind();
	ibo->UnBind();
}

Mesh::Mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, VertexBufferLayout layout) {
	m_vertices = vertices;
	m_indices = indices;
	m_layout = layout;

	GenVerticies();
}

Mesh::Mesh(std::string filePath, VertexBufferLayout layout)
{
	LoadOBJ(filePath);

	m_layout = layout;

	GenVerticies();
}

void Mesh::Bind()const
{
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

Mesh::~Mesh()
{
	delete vbo;
	delete ibo;
	delete vao;
}