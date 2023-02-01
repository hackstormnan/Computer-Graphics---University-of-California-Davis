#include "UI_SimpleEditor.h"
#include "UIManager.h"
#include "EntityManager.h"
#include "EntityInstance.h"
#include "Engine.h"
#include <algorithm>

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

void UI_SimpleEditor::Render()
{
	ImGui::Begin("Simple Editor");

	if (SecondaryOn()) {
		ImGui::End();
		return;
	}
	ImGui::Text("Read and write from Assets/Curves/curveData.txt");
	if (ImGui::Button("Save")) {
		Engine::Get().Save("Assets/Curves/curveData.txt");
	}ImGui::SameLine();
	if (ImGui::Button("Load")) {
		Engine::Get().Load("Assets/Curves/curveData.txt");
	}ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		Engine::Get().Reset();
	}
	ImGui::Text("---------------------------");

	HandleBezier();

	ImGui::Text("---------------------------");

	HandleBspine();

	ImGui::End();
}

void UI_SimpleEditor::Update(float deltaTime)
{
	GetBiziers(Engine::Get().beziers);
	GetBspines(Engine::Get().bspines);
}

void UI_SimpleEditor::GetBiziers(std::vector<std::vector<glm::vec2>>& p_biziers)
{
	biziers = p_biziers;
}

void UI_SimpleEditor::GetBspines(std::vector<std::vector<glm::vec2>>& p_bspines)
{
	bspines = p_bspines;
}

bool UI_SimpleEditor::SecondaryOn()
{
	if (onLevel_1 == false) {
		int width = 100;
		int height = 100;
		Application::Get().GetWindowSize(width, height);
		float w = (float)width / 2;
		float h = (float)height / 2;

		if (HandleBezierPoints(w, h) == false)
			return false;
		if (HandleBspinePoints(w, h) == false)
			return false;
		return true;
	}
	return false;
}

void UI_SimpleEditor::HandleBezier()
{
	ImGui::Text("Bezier curves:");
	if (ImGui::Button("Create Bezier")) {
		Engine::Get().CreateNewBezier();
	}
	for (int i = 0; i < (int)Engine::Get().beziers.size(); i++)
	{
		std::string name = "curve " + std::to_string(i);
		if (ImGui::Button(name.c_str())) {
			selectedBizer = i;
		}ImGui::SameLine();
		std::string deleteButtonName = "Delete curve " + std::to_string(i);
		if (ImGui::Button(deleteButtonName.c_str())) {
			Engine::Get().DeleteBeizer(i);
		}
	}
	if (selectedBizer != -1) {
		onLevel_1 = false;
	}
}

bool UI_SimpleEditor::HandleBezierPoints(float w, float h)
{
	if (selectedBizer != -1) {
		if (ImGui::Button("Go Back")) {
			onLevel_1 = true;
			selectedBizer = -1;
			return false;
		}
		ImGui::Text("Control Points:");
		for (int i = 0; i < (int)Engine::Get().beziers[selectedBizer].size(); i++)
		{
			std::string textName = "P" + std::to_string(i);
			std::string x = textName + " x";
			std::string y = textName + " y";
			ImGui::Text(textName.c_str()); ImGui::SameLine();

			std::string deleteButtonName = "Delete" + textName;//delete point button
			if (ImGui::Button(deleteButtonName.c_str())) {	
				Engine::Get().DeleteBeizerPoint(selectedBizer, i);
				if (Engine::Get().beziers[selectedBizer].size() == 0) {
					Engine::Get().DeleteBeizer(selectedBizer);
					onLevel_1 = true;
					selectedBizer = -1;
					return false;
				}
				if(i != 0)
				i--;
			}ImGui::SameLine();
			
			std::string insertButtonName = "Insert before " + textName;//insert point button
			if (ImGui::Button(insertButtonName.c_str())) {	
				Engine::Get().InsertBeizerPoint(selectedBizer, i);
			}

			ImGui::SliderFloat(x.c_str(), &Engine::Get().beziers[selectedBizer][i].x, -w, w);
			ImGui::SliderFloat(y.c_str(), &Engine::Get().beziers[selectedBizer][i].y, -h, h);
		}
		if (ImGui::Button("Push back")) {
			Engine::Get().CreateBeizerPoint(selectedBizer);
		}
	}
	return true;
}

void UI_SimpleEditor::HandleBspine()
{
	ImGui::Text("B-spine curves:");
	if (ImGui::Button("Create B-spine")) {
		Engine::Get().CreateNewBspine();
	}
	for (int i = 0; i < (int)Engine::Get().bspines.size(); i++)
	{
		std::string name = "b-spine " + std::to_string(i);
		if (ImGui::Button(name.c_str())) {
			selectedBspine = i;
		}ImGui::SameLine();
		std::string deleteButtonName = "Delete b-spine " + std::to_string(i);
		if (ImGui::Button(deleteButtonName.c_str())) {
			Engine::Get().DeleteBspine(i);
		}
	}
	if (selectedBspine != -1) {
		onLevel_1 = false;
	}
}

bool UI_SimpleEditor::HandleBspinePoints(float w, float h)
{
	if (selectedBspine != -1) {
		if (ImGui::Button("Go Back")) {
			onLevel_1 = true;
			selectedBspine = -1;
			return false;
		}
		ImGui::Text("Knot type:");
		KnotType type = Engine::Get().knotTypes[selectedBspine];
		if (type == KnotType::OPEN_END) {
			if (ImGui::Button("Set floating ends")) {
				Engine::Get().knotTypes[selectedBspine] = KnotType::FLOATING_END;
				Engine::Get().ResetKnots(selectedBspine, KnotType::FLOATING_END);
			}
		}
		else {
			if (ImGui::Button("Set open ends")) {
				Engine::Get().knotTypes[selectedBspine] = KnotType::OPEN_END;
				Engine::Get().ResetKnots(selectedBspine, KnotType::OPEN_END);
			}
		}

		ImGui::Text("Degree:");
		int res = Engine::Get().AbleToChangeDegree(selectedBspine);
		if (res == 1 || res == 3) {
			if (ImGui::Button("-")) {
				Engine::Get().AddDegree(selectedBspine, -1);
			}
			ImGui::SameLine();
		} 
		std::string str = " " + std::to_string(Engine::Get().degrees[selectedBspine]) + " ";
		ImGui::Text(str.c_str());	
		if (res == 2 || res == 3) {
			ImGui::SameLine();
			if (ImGui::Button("+")) {
				Engine::Get().AddDegree(selectedBspine, 1);
			}
		}
		if (res == 0)
		{
			ImGui::SameLine();
			ImGui::Text("Cannot modify degree");
		}

		ImGui::Text("Control Points:");

		for (int i = 0; i < (int)Engine::Get().bspines[selectedBspine].size(); i++)
		{
			std::string textName = "P" + std::to_string(i);
			std::string x = textName + " x";
			std::string y = textName + " y";
			ImGui::Text(textName.c_str()); ImGui::SameLine();

			std::string deleteButtonName = "Delete" + textName;//delete point button
			if (ImGui::Button(deleteButtonName.c_str())) {
				Engine::Get().DeleteBspinePoint(selectedBspine, i);
				if (Engine::Get().bspines[selectedBspine].size() == 0) {
					Engine::Get().DeleteBspine(selectedBspine);
					onLevel_1 = true;
					selectedBspine = -1;
					return false;
				}
				if (i != 0)
					i--;
			}ImGui::SameLine();

			std::string insertButtonName = "insert before " + textName;//insert point button
			if (ImGui::Button(insertButtonName.c_str())) {
				Engine::Get().InsertBspinePoint(selectedBspine, i);
			}

			ImGui::SliderFloat(x.c_str(), &Engine::Get().bspines[selectedBspine][i].x, -w, w);
			ImGui::SliderFloat(y.c_str(), &Engine::Get().bspines[selectedBspine][i].y, -h, h);
		}
		if (ImGui::Button("Push back")) {
			Engine::Get().CreateBspinePoint(selectedBspine);
		}
		//knots manipulation
		ImGui::Text("Knots manipulation");
		int n = (int)Engine::Get().bspines[selectedBspine].size() - 1;
		int k = Engine::Get().degrees[selectedBspine];
		int m = n + k + 1;
		for (int i = 0; i <= m; i++)
		{
			std::string name = "Knot " + std::to_string(i);
			ImGui::SliderFloat(name.c_str(), &Engine::Get().knotsList[selectedBspine][i], 0, (float)m);
		}
		std::sort(Engine::Get().knotsList[selectedBspine].begin(), Engine::Get().knotsList[selectedBspine].end());
	}
	return true;
}