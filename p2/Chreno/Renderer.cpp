#include "Renderer.h"
#include <iostream>
void Renderer::GL_ClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool Renderer::GL_LogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "OpenGL Error" << error << " " << function << ":" << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear()const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, DrawMode mode) const
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	glDrawElements(mode, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const Mesh & mesh, const Shader& shader)const
{
	mesh.Bind();
	shader.Bind();
	glDrawElements(GL_TRIANGLES, mesh.GetCount(), GL_UNSIGNED_INT, nullptr);
}
