#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_id;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer * vb, VertexBufferLayout& layout);
	void Bind()const;
	void UnBind();
};