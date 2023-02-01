#include "MathUtil.h"

glm::vec3 MathUtil::Normalize(const glm::vec3 v)
{
	float s = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	float x = v.x / s;
	float y = v.y / s;
	float z = v.z / s;
	return glm::vec3(x, y, z);
}

float MathUtil::Dot(const glm::vec3 v, const glm::vec3 w)
{
	glm::vec3 res = glm::vec3(v * w);
	return res.x + res.y + res.z;
}

glm::vec3 MathUtil::Cross(const glm::vec3 x, const glm::vec3 y)
{
	return glm::vec3(
		x.y * y.z - y.y * x.z, 
		x.z * y.x - y.z * x.x,
		x.x * y.y - y.x * x.y
	);
}

glm::mat4 MathUtil::Ortho(float left, float right, float bottom, float top, float near, float far)
{
	glm::mat4 res(1);
	res[0][0] = 2.0f / (right - left);
	res[1][1] = 2.0f / (top - bottom);
	res[2][2] = -2.0f / (far - near);
	res[3][0] = -(right + left) / (right - left);
	res[3][1] = -(top + bottom) / (top - bottom);
	res[3][2] = -(far + near) / (far - near);
	return res;
}

glm::mat4 MathUtil::Pespective(float view, float ratio, float near, float far)
{
	glm::mat4 res(0);

	float t_halfView = tan(view / 2.0f);
	res[0][0] = 1.0f / (ratio * t_halfView);
	res[1][1] = 1.0f / (t_halfView);
	res[2][2] = -(far + near) / (far - near);
	res[2][3] = -1.0f;
	res[3][2] = -(2.0f * far * near) / (far - near);
	return res;
}
