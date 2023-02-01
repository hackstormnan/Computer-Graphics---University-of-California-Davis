#include "UI_TransformEditor.h"
#include "UIManager.h"
#include "EntityManager.h"

void UI_WindowBase::Render()
{
	static float f = 0.0f;
	static int counter = 0;
	static bool show_demo_window;
	static bool show_another_window;
	ImGui::Begin("Hello, world!");                          
	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void UI_WindowBase::Update(float deltaTime)
{
}

void UI_TransformEditor::Render()
{
	ImGui::Begin("Transform Editor");

	if (selectedEntity != nullptr) {
		ImGui::Text("Name: ", selectedEntity);
		ImGui::SameLine();
		ImGui::Text(selectedEntity->GetName().c_str());
		ImGui::Text(" ");
		//position
		ImGui::Text("Position: ");
		glm::vec3& pos = selectedEntity->transform.m_position;
		ImGui::SliderFloat("pos x", &pos.x, -3.0f, 3.0f);
		ImGui::SliderFloat("pos y", &pos.y, -3.0f, 3.0f);
		ImGui::SliderFloat("pos z", &pos.z, -3.0f, 3.0f);
		ImGui::Text(" ");

		//euler angle
		ImGui::Text("Euler Angle: ");
		glm::vec3& angle = selectedEntity->transform.m_eulerAngles;
		ImGui::SliderFloat("euler x", &angle.x, -180.0f, 180.0f);
		ImGui::SliderFloat("euler y", &angle.y, -180.0f, 180.0f);
		ImGui::SliderFloat("euler z", &angle.z, -180.0f, 180.0f);
		ImGui::Text(" ");

		//scale
		ImGui::Text("Scale: ");
		glm::vec3& scale = selectedEntity->transform.m_scale;
		ImGui::SliderFloat("scale x", &scale.x, -3.0f, 3.0f);
		ImGui::SliderFloat("scale y", &scale.y, -3.0f, 3.0f);
		ImGui::SliderFloat("scale z", &scale.z, -3.0f, 3.0f);
		ImGui::Text(" ");

	}

	ImGui::Text("Entity List: ");
	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		if (ImGui::Button(m_entities[i]->GetName().c_str())) {
			for (unsigned int j = 0; j < m_entities.size(); j++)
			{
				m_entities[j]->m_enabled = false;
			}
			selectedEntity = m_entities[i]; 
			m_entities[i]->m_enabled = true;
		}
	}

	ImGui::End();
}

void UI_TransformEditor::Update(float deltaTime)
{
	m_entities = EntityManager::Get().GetAllEntities();
}