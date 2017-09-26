#include "WinConfiguration.h"
#include "Imgui/imgui.h"

WinConfiguration::WinConfiguration() : EditorWindow()
{

}

WinConfiguration::~WinConfiguration()
{

}

void WinConfiguration::Update()
{
	if (ImGui::Begin("Configuration"))
	{
		ImGui::Text("Options");

		if (ImGui::BeginMenu("Application"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::EndMenu();
		}

		ImGui::End();
	}
}