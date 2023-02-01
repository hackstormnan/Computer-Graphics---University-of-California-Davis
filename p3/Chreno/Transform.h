#pragma once
#include "GL_Dependencies.h"
class Transform
{
public:
	Transform();
	~Transform();

	glm::vec3 GetRight()const;
	glm::vec3 GetUp()const;
	glm::vec3 GetForward()const;

	glm::mat4 GetWorldMatrix()const;

	glm::mat4 GetTranslateMatrix(const glm::vec3& v)const;
	glm::mat4 GetScaleMatrix(const glm::vec3& v)const;
	glm::mat4 GetEulerMatrix(const glm::vec3& v)const;

	void CalWorldMatrix();

	glm::vec3 m_position;
	glm::vec3 m_eulerAngles;
	glm::vec3 m_scale;
private:
	glm::mat4 GetRotateMatrix(const glm::mat4 m, const float angle, const glm::vec3 v)const;
	glm::mat4 m_worldMatrix;
};