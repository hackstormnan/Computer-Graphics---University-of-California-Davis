#pragma once
#include <vector>
#include "GL/glew.h"
struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_BYTE:			return 1;
		default:
			return -1;
			break;
		}
	}
};

class VertexBufferLayout
{
private:
	unsigned int m_stride;
	std::vector<VertexBufferElement> m_elements;
public:

	VertexBufferLayout() = default;
	template<typename T>
	void Push(T count) ;


	inline const std::vector<VertexBufferElement> GetElements() { return m_elements; }
	inline unsigned int GetStride() const {	return m_stride; }
};
