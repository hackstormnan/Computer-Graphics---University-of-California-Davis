#pragma once
#include "GL_Dependencies.h"
#include "Singleton.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"

#define GL_CALL(X) GL_ClearError();\
	X;\
	ASSERT(GL_LogCall(#X,__FILE__,__LINE__))

enum DrawMode {
	TRIANGLE = GL_TRIANGLES,
	LINE = GL_LINES
};

class Renderer: public Singleton<Renderer>
{
	friend class Singleton<Renderer>;
public:
	void GL_ClearError();

	bool GL_LogCall(const char* function, const char* file, int line);

	void Clear()const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, DrawMode mode = DrawMode::TRIANGLE) const;
	void Draw(const Mesh& mesh, const Shader& shader, DrawMode mode = DrawMode::TRIANGLE)const;
	void DrawByFaces(const Mesh& mesh, const Shader& shader, Camera* cam, DrawMode mode = DrawMode::TRIANGLE)const;

	void TransferToHalfTone();

	glm::vec3 GetPixelColor(GLint x, GLint y);
	void SetPixel(int x, int y, float r, float g, float b);
	int GetPixelLocation(int x, int y);
private:
	float * m_pixelBuffer = new float[WindowProperty::width * WindowProperty::height * 3];
};