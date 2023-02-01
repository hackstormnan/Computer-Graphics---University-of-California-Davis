#include "VertexBufferLayout.h"
	template <>
	void VertexBufferLayout::Push<float>(float count) {
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += sizeof(GLfloat) * count;
	}

	template <>
	void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += sizeof(GLuint) * count;
	}

	template <>
	void VertexBufferLayout::Push<unsigned char>(unsigned char count) {
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += sizeof(GLbyte) * count;
	}
