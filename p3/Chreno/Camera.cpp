#include "Camera.h"
#include "Singleton.h"
#include "Transform.h"
#include "Application.h"
#include "InputManager.h"
#include "MathUtil.h"
Camera::Camera()
{
	m_view = glm::mat4(1.0);
	m_projection = glm::mat4(1.0);

	m_projectionMode = ProjectionMode::PERSPECTIVE;

	m_viewAngle = 60.0f;
	m_viewSize = 1.5f;
	m_near = -3.0f;
	m_far = 1000.0f;

	viewPort_x = 0;
	viewPort_y = 0;
	viewPort_width = WindowProperty::width;
	viewPort_height = WindowProperty::height;
}

void Camera::Render()
{
	glViewport((int)viewPort_x, (int)viewPort_y, (int)viewPort_width, (int)viewPort_height);
	m_view = glm::mat4(1.0);
	m_projection = glm::mat4(1.0);

	if (m_projectionMode == ProjectionMode::PERSPECTIVE)
	{
		m_projection = MathUtil::Pespective(glm::radians(m_viewAngle), viewPort_width / viewPort_height, 0.0f, m_far);
	}
	else if (m_projectionMode == ProjectionMode::ORTHOGRAPHIC)
	{
		glm::vec4 view = glm::vec4(-(viewPort_width / viewPort_height), (viewPort_width / viewPort_height), -1.0f, 1.0f) * m_viewSize;
		m_projection = MathUtil::Ortho(view.x, view.y, view.z, view.w, m_near, m_far);
	}
}

void Camera::Update(float p_deltaTime)
{
	transform.CalWorldMatrix();
	Move(p_deltaTime);
	Rotate(p_deltaTime);
}

void Camera::Move(float p_deltaTime)
{
	glm::vec3 t_direction = glm::vec3(0.0);

	if (InputManager::Get().IsKeyDown(Keycode::KEYCODE_A)) { t_direction += transform.GetRight(); }
	if (InputManager::Get().IsKeyDown(Keycode::KEYCODE_D)) { t_direction -= transform.GetRight(); }
	if (InputManager::Get().IsKeyDown(Keycode::KEYCODE_E)) { t_direction += transform.GetUp(); }
	if (InputManager::Get().IsKeyDown(Keycode::KEYCODE_Q)) { t_direction -= transform.GetUp(); }
	if (InputManager::Get().IsKeyDown(Keycode::KEYCODE_W)) { t_direction += transform.GetForward(); }
	if (InputManager::Get().IsKeyDown(Keycode::KEYCODE_S)) { t_direction -= transform.GetForward(); }

	transform.m_position += t_direction * p_deltaTime * m_moveSpeed;
}

void Camera::Rotate(float p_deltaTime)
{
	glm::vec2 mouse_position =
	{
		InputManager::Get().GetMousePosition().x,
		InputManager::Get().GetMousePosition().y
	};

	if (InputManager::Get().IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		const glm::vec2 mouse_delta =
		{
		   mouse_position.x - prev_mouse_position.x,
		   mouse_position.y - prev_mouse_position.y,
		};
		prev_mouse_position = mouse_position;

		if (mouse_delta.x != 0.0f)
		{
			transform.m_eulerAngles.y -= p_deltaTime * mouse_delta.x * mouse_yaw_factor * mouse_sensitivity;
		}
		if (mouse_delta.y != 0.0f)
		{
			transform.m_eulerAngles.x -= p_deltaTime * mouse_delta.y * mouse_pitch_factor * mouse_sensitivity;
		}
	}
	else
	{
		prev_mouse_position = mouse_position;
	}
}


glm::mat4 Camera::GetViewMatrix()
{
	glm::vec3 position = transform.m_position;
	glm::vec3 target = transform.m_position + transform.GetForward();
	glm::vec3 up = transform.GetUp();

	glm::vec3 f(MathUtil::Normalize(target - position));
	glm::vec3 s(MathUtil::Normalize(MathUtil::Cross(f, up)));
	glm::vec3 u(MathUtil::Cross(s, f));

	glm::mat4 res(1);
	res[0][0] = s.x;
	res[1][0] = s.y;
	res[2][0] = s.z;
	res[0][1] = u.x;
	res[1][1] = u.y;
	res[2][1] = u.z;
	res[0][2] = -f.x;
	res[1][2] = -f.y;
	res[2][2] = -f.z;
	res[3][0] = -MathUtil::Dot(s, position);
	res[3][1] = -MathUtil::Dot(u, position);
	res[3][2] =  MathUtil::Dot(f, position);

	return res;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return m_projection;
}

void Camera::SetProjectionMode(ProjectionMode p_mode)
{
	m_projectionMode = p_mode;
}

void Camera::SetViewPort(float x, float y, float width, float height)
{
	viewPort_x = x;
	viewPort_y = y;
	viewPort_width = width;
	viewPort_height = height;
}