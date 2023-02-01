#include "VertexArray.h"
#include "Renderer.h"
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

void VertexArray::AddBuffer(VertexBuffer * vb, VertexBufferLayout& layout)
{
	this->Bind();
	vb->  Bind();
	const auto elements = layout.GetElements();
	unsigned int offset = 0;

	ASSERT(glGetError() == GL_NO_ERROR);
	for (unsigned i = 0; i < elements.size(); i++)
	{
		const auto & element = elements[i];
		glEnableVertexAttribArray(i);

		ASSERT(glGetError() == GL_NO_ERROR);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void * )offset);	//this bind vao and vbo

		ASSERT(glGetError() == GL_NO_ERROR);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind()const
{
	glBindVertexArray(m_id);
}

void VertexArray::UnBind()
{
	glBindVertexArray(0);
}
