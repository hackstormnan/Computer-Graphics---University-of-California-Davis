#pragma once
//ImGUI
#include "GL_Dependencies.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imconfig.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Singleton.h"
#include "UI_TransformEditor.h"

class UIManager : public Singleton<UIManager>
{
	friend class Singleton<UIManager>;
public:
	bool Init(GLFWwindow* window);
	void Update(float deltaTime);
	void Render();
	void ShutDown();

	UI_TransformEditor m_transformWindow;
private:
	GLFWwindow * window;
};