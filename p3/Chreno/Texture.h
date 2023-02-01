#pragma once
#include "Renderer.h"
#include "Image.h"
class Texture
{
public:
	Texture(std::string fileName);
	Texture() = default;
	~Texture();

	void Bind(int slot = 0);
	void UnBind();

	unsigned int m_id;
	Image * m_image;
};