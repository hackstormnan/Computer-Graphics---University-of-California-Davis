#include "Texture.h"

Texture::Texture(std::string fileName)
{
	m_image = new Image(fileName.c_str());

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image->GetWidth(), m_image->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image->GetBuffer());
	glBindTexture(GL_TEXTURE_2D, 0);
	
	delete m_image; //data will be freed in destructor
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Texture::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
