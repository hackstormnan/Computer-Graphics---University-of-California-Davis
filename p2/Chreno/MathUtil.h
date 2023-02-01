#pragma once
#include "GL_Dependencies.h"

class MathUtil
{
public:
	static glm::vec3 Normalize(const glm::vec3 v);
	static float Dot(const glm::vec3 x, const glm::vec3 y);
	static glm::vec3 Cross(const glm::vec3 x, const glm::vec3 y);
	static glm::mat4 Ortho(float left, float right, float bottem, float top, float near, float far);
	static glm::mat4 Pespective(float view, float ratio, float near, float far);
};