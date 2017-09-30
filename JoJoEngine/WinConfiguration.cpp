#include "WinConfiguration.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

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
			bool bool_tmp;
			int  int_tmp;

			int_tmp = App->window->GetWidth();
			if (ImGui::SliderInt("Width", &int_tmp, 0, 2000))
			{
				App->window->SetWidth(int_tmp);
			}

			int_tmp = App->window->GetHeight();
			if (ImGui::SliderInt("Height", &int_tmp, 0, 2000))
			{
				App->window->SetHeight(int_tmp);
			}


			bool_tmp = App->window->IsFullscreen();
			if (ImGui::Checkbox("Fullscreen", &bool_tmp))
			{
				App->window->SetFullscreen(bool_tmp);
			}

			ImGui::SameLine();

			bool_tmp = App->window->IsResizable();
			if (ImGui::Checkbox("Resizable", &bool_tmp))
			{
				App->window->SetResizable(bool_tmp);
			}

			bool_tmp = App->window->IsBorderless();
			if (ImGui::Checkbox("Borderless", &bool_tmp))
			{
				App->window->SetBorderless(bool_tmp);
			}

			ImGui::SameLine();

			bool_tmp = App->window->IsFullScreenDesktop();
			if (ImGui::Checkbox("Fullscreen desktop", &bool_tmp))
			{
				App->window->SetFullScreenDesktop(bool_tmp);
			}
		}

		ImGui::End();
	}
}