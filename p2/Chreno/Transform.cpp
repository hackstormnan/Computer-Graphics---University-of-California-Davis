#include "Transform.h"
#include "MathUtil.h"
Transform::Transform()
{
	m_eulerAngles = glm::vec3(0.0);
	m_position = glm::vec3(0.0);
	m_scale = glm::vec3(1.0);
}

glm::mat4 Transform::GetTranslateMatrix(const glm::vec3& v)const
{
	glm::mat4 m = glm::mat4(1.0);

	m[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

	return m;
}

glm::mat4 Transform::GetScaleMatrix(const glm::vec3& v)const
{
	glm::mat4 m = glm::mat4(1.0);

	m[0] = m[0] * v[0];
	m[1] = m[1] * v[1];
	m[2] = m[2] * v[2];
	m[3] = m[3];

	return m;
}

glm::mat4 Transform::GetEulerMatrix(const glm::vec3& v)const
{
	glm::mat4 m = glm::mat4(1.0);

	//pitch
	m = GetRotateMatrix(m, glm::radians(v.x), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec4 y = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	y = y * m;
	//yaw
	m = GetRotateMatrix(m, glm::radians(v.y), glm::vec3(y.x, y.y, y.z));

	glm::vec4 z = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	z = z * m;
	//roll
	m = GetRotateMatrix(m, glm::radians(v.z), glm::vec3(z.x, z.y, z.z));

	return m;
}

glm::mat4 Transform::GetRotateMatrix(const glm::mat4 m, const float angle, const glm::vec3 v)const
{
	float t_cos = std::cos(angle);
	float t_sin = std::sin(angle);

	glm::vec3 t_axis(MathUtil::Normalize(v));
	glm::vec3 temp((1 - t_cos) * t_axis);

	glm::mat4 t_rot;
	t_rot[0][0] = t_cos + temp[0] * t_axis[0];
	t_rot[0][1] = temp[0] * t_axis[1] + t_sin * t_axis[2];
	t_rot[0][2] = temp[0] * t_axis[2] - t_sin * t_axis[1];

	t_rot[1][0] = temp[1] * t_axis[0] - t_sin * t_axis[2];
	t_rot[1][1] = t_cos + temp[1] * t_axis[1];
	t_rot[1][2] = temp[1] * t_axis[2] + t_sin * t_axis[0];

	t_rot[2][0] = temp[2] * t_axis[0] + t_sin * t_axis[1];
	t_rot[2][1] = temp[2] * t_axis[1] - t_sin * t_axis[0];
	t_rot[2][2] = t_cos + temp[2] * t_axis[2];

	glm::mat4 res;
	res[0] = m[0] * t_rot[0][0] + m[1] * t_rot[0][1] + m[2] * t_rot[0][2];
	res[1] = m[0] * t_rot[1][0] + m[1] * t_rot[1][1] + m[2] * t_rot[1][2];
	res[2] = m[0] * t_rot[2][0] + m[1] * t_rot[2][1] + m[2] * t_rot[2][2];
	res[3] = m[3];

	return res;
}

glm::mat4 Transform::GetWorldMatrix()const
{
	glm::mat4 posMatrix = GetTranslateMatrix(m_position);
	glm::mat4 scaleMatrix = GetScaleMatrix(m_scale);
	glm::mat4 eulerMatrix = GetEulerMatrix(m_eulerAngles);

	glm::mat4 m_worldMatrix = glm::mat4(1.0) * posMatrix * scaleMatrix * eulerMatrix;

	return m_worldMatrix;
}

glm::vec3 Transform::GetRight()const
{
	glm::vec4 result = GetEulerMatrix(m_eulerAngles) * glm::vec4(1, 0, 0, 0);
	return glm::vec3{ result.x,result.y,result.z };
}

glm::vec3 Transform::GetUp()const
{
	glm::vec4 result = GetEulerMatrix(m_eulerAngles) * glm::vec4(0, 1, 0, 0);
	return glm::vec3{ result.x,result.y,result.z };
}

glm::vec3 Transform::GetForward()const
{
	glm::vec4 result = GetEulerMatrix(m_eulerAngles) * glm::vec4(0, 0, 1, 0);
	return glm::vec3{ result.x,result.y,result.z };
}

Transform::~Transform()
{

}