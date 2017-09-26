#include "WinConfiguration.h"
#include "Application.h"
#include "ModuleEditor.h"

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
		
		ImGui::PushItemWidth(-140);
		ImGui::Text("Options");

		if (ImGui::CollapsingHeader("Application"))
		{
			char tmp_str[100];

			//App name
			strcpy_s(tmp_str, sizeof(tmp_str), App->GetName().c_str());
			if (ImGui::InputText("Name", tmp_str, sizeof(tmp_str)))
			{
				App->SetName(tmp_str);
			}

			//Organization name
			strcpy_s(tmp_str, sizeof(tmp_str), App->GetOrganization().c_str());
			if (ImGui::InputText("Organization", tmp_str, sizeof(tmp_str)))
			{
				App->SetOrganization(tmp_str);
			}
			
			//sprintf_s(tmp_str, sizeof(tmp_str), "Framerate %.1f")
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			
		}

		ImGui::End();
	}
}