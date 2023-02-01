#include "InputManager.h"

InputManager::InputManager()
{
	m_keyboard = new Keyboard();
	m_mouse = new Mouse();
}

InputManager::~InputManager()
{
	delete m_keyboard;
	delete m_mouse;
}

bool InputManager::IsKeyDown(Keycode p_eKey)
{
	return m_keyboard->currentKeys[p_eKey];
}

bool InputManager::IsKeyPressed(Keycode p_eKey)
{
	return m_keyboard->currentKeys[p_eKey] && !m_keyboard->previousKeys[p_eKey];
}

bool InputManager::IsKeyReleased(Keycode p_eKey)
{
	return !m_keyboard->currentKeys[p_eKey] && m_keyboard->previousKeys[p_eKey];
}

bool InputManager::IsMouseButtonDown(MouseButton p_iButton)
{
	return m_mouse->currentButtons[p_iButton];
}

bool InputManager::IsMouseButtonPressed(MouseButton p_iButton)
{
	return m_mouse->currentButtons[p_iButton] && !m_mouse->previousButtons[p_iButton];
}

bool InputManager::IsMouseButtonReleased(MouseButton p_iButton)
{
	return !m_mouse->currentButtons[p_iButton] && m_mouse->previousButtons[p_iButton];
}

glm::vec2 InputManager::GetMousePosition()
{
	glm::vec2 mousePosition = glm::vec2(m_mouse->x, m_mouse->y);

	return mousePosition;
}

Keyboard * InputManager::GetKeyboard()
{
	return m_keyboard;
}

Mouse * InputManager::GetMouse()
{
	return m_mouse;
}

void InputManager::InputHandle(GLFWwindow* window)
{
	Keyboard* keyboard = InputManager::Get().GetKeyboard();
	Mouse* mouse = InputManager::Get().GetMouse();
	for (int i = 0; i < Keycode::KEYCODE_COUNT; i++)
	{
		keyboard->previousKeys[i] = keyboard->currentKeys[i];
		keyboard->currentKeys[i] = glfwGetKey(window, i) ? GLFW_PRESS : GLFW_RELEASE;
	}

	for (int i = 0; i < MouseButton::MOUSE_BUTTON_COUNT; i++)
	{
		mouse->previousButtons[i] = mouse->currentButtons[i];
		mouse->currentButtons[i] = glfwGetMouseButton(window, i) ? GLFW_PRESS : GLFW_RELEASE;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	mouse->x = (int)x;
	mouse->y = (int)y;
}