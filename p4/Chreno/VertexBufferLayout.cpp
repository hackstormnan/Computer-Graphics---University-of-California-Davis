#include "VertexBufferLayout.h"
template <>
void VertexBufferLayout::Push<float>(float count) {
	m_elements.push_back({ GL_FLOAT, (unsigned int)count, GL_FALSE });
	m_stride += sizeof(GLfloat) * (unsigned int)count;
}

template <>
void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
	m_elements.push_back({ GL_UNSIGNED_INT, (unsigned int)count, GL_FALSE });
	m_stride += sizeof(GLuint) * (unsigned int)count;
}

template <>
void VertexBufferLayout::Push<unsigned char>(unsigned char count) {
	m_elements.push_back({ GL_UNSIGNED_BYTE, (unsigned int)count, GL_TRUE });
	m_stride += sizeof(GLbyte) * (unsigned int)count;
}
