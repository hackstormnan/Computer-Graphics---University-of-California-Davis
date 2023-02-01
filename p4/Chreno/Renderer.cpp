#include "Renderer.h"
#include "Application.h"
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
void Renderer::DrawPixel(float x, float y, glm::vec3 color)
{
	int width = 100;
	int height = 100;
	Application::Get().GetWindowSize(width, height);

	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	glVertex3f(x * 2 / (float)width, y * 2 / (float)height, 0);
	glEnd();
}
void Renderer::DrawLine(glm::vec2 p1, glm::vec2 p2, glm::vec3 color)
{
	int width = 100;
	int height = 100;
	Application::Get().GetWindowSize(width, height);

	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex3f(p1.x * 2 / (float)width, p1.y * 2 / (float)height, 0);
	glVertex3f(p2.x * 2 / (float)width, p2.y * 2 / (float)height, 0);
	glEnd();
}

int Renderer::GetPixelLocation(int x, int y)
{
	int width = WindowProperty::width;
	return (y * width * 3) + (x * 3);
}

glm::vec2 Renderer::GetInterpolation(glm::vec2 p1, glm::vec2 p2, float t)
{
	if (t < 0) {
		return GetInterpolation(p1, p2, 0);
	}
	if (t > 1) {
		return GetInterpolation(p1, p2, 1);
	}

	glm::vec2 result = (1 - t) * p1 + t * p2;
	return result;
}

glm::vec2 Renderer::GetDeCasteljau(std::vector<glm::vec2> controlPoints, int i, int j, float t)
{
	glm::vec2 result = glm::vec2(0);

	if (j == 0) {
		return controlPoints[i];
	}
	else {
		return (1 - t) * GetDeCasteljau(controlPoints, i, j - 1, t) + t * GetDeCasteljau(controlPoints, i + 1, j - 1, t);
	}
	return result;
}

void Renderer::DrawBezierCurve(std::vector<glm::vec2> controlPoints, int lineNum)
{
	for (int i = 0; i < (int)controlPoints.size(); i++)
	{
		if (i != controlPoints.size() - 1)
			DrawLine(controlPoints[i], controlPoints[i + 1], glm::vec3(0, 0.5, 0));
		DrawDebugPoint(controlPoints[i], glm::vec3(0, 1, 0));
	}

	float t_perLine = 1 / (float)lineNum;
	
	float t1 = 0;
	float t2 = 0;
	int j = (int)controlPoints.size() - 1;
	glm::vec2 p1 = GetDeCasteljau(controlPoints, 0, j, t1);
	glm::vec2 p2 = glm::vec2(0);
	for (int i = 0; i <= lineNum - 1; i++)
	{
		t2 = (i + 1) * t_perLine;
		glm::vec2 p2 = GetDeCasteljau(controlPoints, 0, j, t2);
		DrawLine(p1, p2);
		t1 = t2;
		p1 = p2;
	}
}

void Renderer::DrawDebugPoint(glm::vec2 p1, glm::vec3 color)
{
	DrawPixel(p1.x	  , p1.y    ,color);
	DrawPixel(p1.x - 1, p1.y	,color);
	DrawPixel(p1.x + 1, p1.y	,color);
	DrawPixel(p1.x	  , p1.y + 1,color);
	DrawPixel(p1.x - 1, p1.y + 1,color);
	DrawPixel(p1.x + 1, p1.y + 1,color);
	DrawPixel(p1.x	  , p1.y - 1,color);
	DrawPixel(p1.x - 1, p1.y - 1,color);
	DrawPixel(p1.x + 1, p1.y - 1,color);
}

void Renderer::DrawBspineCurve(std::vector<glm::vec2> controlPoints, std::vector<float> knots, int degree, int lineNum)
{
	for (int i = 0; i < (int)controlPoints.size(); i++)
	{
		if (i != controlPoints.size() - 1)
			DrawLine(controlPoints[i], controlPoints[i + 1], glm::vec3(0.5, 0.5, 0));
		DrawDebugPoint(controlPoints[i], glm::vec3(1, 1, 0));
	}
	if (controlPoints.size() <= 2) {
		return;
	}

	int n = (int)controlPoints.size() - 1;
	int k = degree;
	int m = n + k + 1;

	std::vector<float> baseFuncsT1; 
	std::vector<float> baseFuncsT2; 
	
	float t_perLine = (knots[m - k] - knots[k]) / (float)lineNum;

	float t1 = knots[k];
	float t2 = knots[k];
	for (int i = 0; i < (int)controlPoints.size(); i++)
	{
		baseFuncsT1.push_back(DeBoorBaseFunc(knots, i, k, t1));
	}
	glm::vec2 p1 = GetDeBoor(controlPoints, baseFuncsT1);
	glm::vec2 p2 = glm::vec2(0);
	for (int i = 0; i <= lineNum - 1; i++)
	{
		t2 = knots[k] + (i + 1) * t_perLine;
		for (int i = 0; i < (int)controlPoints.size(); i++)
		{
			baseFuncsT2.push_back(DeBoorBaseFunc(knots, i, k, t2));
		}
		glm::vec2 p2 = GetDeBoor(controlPoints, baseFuncsT2);
		if(p1 != glm::vec2(0) && p2 != glm::vec2(0))
		DrawLine(p1, p2);
		baseFuncsT1.clear();
		baseFuncsT2.clear();
		t1 = t2;
		for (int i = 0; i < (int)controlPoints.size(); i++)
		{
			baseFuncsT1.push_back(DeBoorBaseFunc(knots, i, k, t1));
		}
		p1 = p2;
	}
}

float Renderer::DeBoorBaseFunc(std::vector<float> knots, int i, int j, float t)
{
	knots.back() += 0.000001f;	//avoid 0/0 given nan(ind)
	if (j == 0) {
		if (t >= knots[i] && t < knots[i + 1])
			return 1.0f;
		else
			return 0.0f;
	}
	else {
		float mole1 = t - knots[i];
		float deno1 = knots[i + j] - knots[i];
		float sub1 = DeBoorBaseFunc(knots, i, j - 1, t);

		float mole2 = knots[i + j + 1] - t;
		float deno2 = knots[i + j + 1] - knots[i + 1];
		float sub2 = DeBoorBaseFunc(knots, i + 1, j - 1, t);
		
		float n1 = mole1 / deno1 * sub1;
		float n2 = mole2 / deno2 * sub2;

		if (deno1 * sub1 == 0) {	//avoid 0/0 given nan(ind)
			n1 = 0;
		}
		if (deno2 * sub2 == 0) {	//avoid 0/0 given nan(ind)
			n2 = 0;
		}

		//return fminf(n1 + n2, 1.0f);
		return n1 + n2;
	}
}

glm::vec2 Renderer::GetDeBoor(std::vector<glm::vec2> controlPoints, std::vector<float> baseFunc)
{
	glm::vec2 result = glm::vec2(0);
	for (int i = 0; i < (int)controlPoints.size(); i++)
	{
		result += controlPoints[i] * baseFunc[i];
	}
	return result;
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