#include "Image.h"
#include "stb_image.h"

Image::Image(const char * p_path)
	: m_width(0)
	, m_height(0)
	, m_bytesPerPixel(0)
	, m_localBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(p_path, &m_width, &m_height, &m_bytesPerPixel, 4);
}

Image::~Image()
{
	stbi_image_free(m_localBuffer);
}

int Image::GetWidth()
{
	return m_width;
}

int Image::GetHeight()
{
	return m_height;
}

int Image::GetBytesPerPixel()
{
	return m_bytesPerPixel;
}

unsigned char* Image::GetBuffer()
{
	return m_localBuffer;
}
