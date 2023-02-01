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
	glColor3f(1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, DrawMode mode) const
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	glDrawElements(mode, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const Mesh & mesh, const Shader& shader, DrawMode mode)const
{
	mesh.Bind();
	shader.Bind();
	glDrawElements(mode, mesh.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawByFaces(const Mesh & mesh, const Shader& shader, Camera* cam, DrawMode mode) const
{
	mesh.SortFaces(cam);
	for (int i = 0; i < mesh.GetFaceCount(); i++)	//draw one face at a time
	{
		mesh.BindFace(i);
		shader.Bind();
		glDrawElements(mode, 3, GL_UNSIGNED_INT, nullptr);
	}
}

glm::vec3 Renderer::GetPixelColor(GLint x, GLint y)
{
	glm::vec3 color;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
	return color;
}

void Renderer::SetPixel(int x, int y, float r, float g, float b)
{
	int t_location = GetPixelLocation(x,y);

	m_pixelBuffer[t_location] = r;
	m_pixelBuffer[t_location + 1] = g;
	m_pixelBuffer[t_location + 2] = b;
}
int Renderer::GetPixelLocation(int x, int y)
{
	int width = WindowProperty::width;
	return (y * width * 3) + (x * 3);
}

void Renderer::TransferToHalfTone()
{
	int width = WindowProperty::width;
	int height = WindowProperty::height;

	glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, m_pixelBuffer);
	
	int x, y;
	int m[3][3] = { {4,  9,  2},	//sum in column and row are all 15, which are (1 + 2 + ... + 9) / 3
				  {  3,  5,  7},
				  {  8,  1,  6}};

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int location = GetPixelLocation(i, j);
			float r = m_pixelBuffer[location];
			float g = m_pixelBuffer[location + 1];
			float b = m_pixelBuffer[location + 2];
			x = i % 3;
			y = j % 3;
			r = r * 9;
			g = g * 9;
			b = b * 9;
			if (r < m[y][x]) r = 0;	else r = 1;
			if (g < m[y][x]) g = 0;	else g = 1;
			if (b < m[y][x]) b = 0;	else b = 1;
			m_pixelBuffer[location] = r;
			m_pixelBuffer[location + 1] = g;
			m_pixelBuffer[location + 2] = b;
		}
	}

	glDrawPixels(width, height, GL_RGB, GL_FLOAT, m_pixelBuffer);
	glFlush();
}