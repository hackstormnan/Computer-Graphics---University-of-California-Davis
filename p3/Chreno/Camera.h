#pragma once
#include "Entity.h"
#include "GL_Dependencies.h"

enum ProjectionMode
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

class Camera : public Entity
{
public:
	void Update(float p_deltaTime)override;
	void Render()override;

	Camera();
	~Camera() = default;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void SetProjectionMode(ProjectionMode p_mode);
	void SetViewPort(float x, float y, float width, float height);
private:
	glm::mat4 m_view;
	glm::mat4 m_projection;

	ProjectionMode m_projectionMode;
	float m_viewSize;
	float m_viewAngle;
	float m_near;
	float m_far;

	float viewPort_x;
	float viewPort_y;
	float viewPort_width;
	float viewPort_height;

	void Move(float p_deltaTime);
	void Rotate(float p_deltaTime);

	float m_moveSpeed = 10.0f;
	float mouse_sensitivity = 6.0f;
	float mouse_yaw_factor = 0.5f;
	float mouse_pitch_factor = -0.5f;
	glm::vec2 prev_mouse_position;
};