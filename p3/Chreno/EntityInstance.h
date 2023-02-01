#pragma once
#include "Entity.h"
#include <string>
#include "Camera.h"
#include "Texture.h"

enum DrawType {
	DEFAULT,
	PAINTERS_ALGORITHEM
};

class EntityInstance : public Entity
{
public:
	EntityInstance(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::string shaderPath, std::string texPath, DrawType p_type = DrawType::DEFAULT);
	EntityInstance(std::string meshPath, std::string filePath, std::string texPath, DrawType p_type = DrawType::DEFAULT);
	void Render()override;
	void Update(float deltaTime)override;

	Camera* camera;

	std::vector<vertex> GetVertices();
	std::vector<unsigned int> GetIndices();
	void UpdateShader(std::string p_path);
	void UpdateDrawType(DrawType p_drawType);
private:
	std::string m_shaderPath;
	std::string m_meshPath;
	std::string m_texPath;

	Texture* m_texture;
	Mesh * m_mesh;
	DrawType m_drawType;

	VertexBufferLayout m_layout;

	std::vector<vertex> m_vertices;
	std::vector<unsigned int> m_indices;
};