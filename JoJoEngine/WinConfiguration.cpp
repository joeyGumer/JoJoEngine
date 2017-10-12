#include "WinConfiguration.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Imgui/imgui.h"

WinConfiguration::WinConfiguration() : EditorWindow()
{

}

WinConfiguration::~WinConfiguration()
{

}

void WinConfiguration::Start()
{
	fps.StartBar(1000);
	ms.StartBar(1);
}

void WinConfiguration::Update()
{
	fps.FillBar(App->GetFPS());
	ms.FillBar(App->GetMs());

	if (is_open)
	{
		ImGui::Begin("Configuration", &is_open);

		ImGui::PushItemWidth(-140);
		ImGui::Text("Options");

		TabApplication();
		TabWindow();	
		TabRenderer();

		ImGui::End();
	}
}

void WinConfiguration::TabApplication()
{
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

		//Performance
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps.GetData()[fps.GetData().size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps.GetData()[0], fps.GetData().size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", ms.GetData()[ms.GetData().size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms.GetData()[0], ms.GetData().size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

		//sprintf_s(tmp_str, sizeof(tmp_str), "Framerate %.1f")
	}
}

void WinConfiguration::TabWindow()
{
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
}

void WinConfiguration::TabRenderer()
{

	if (ImGui::CollapsingHeader("Renderer"))
	{
		bool bool_tmp;

		bool_tmp = App->renderer3D->draw_normals;
		if (ImGui::Checkbox("Draw Normals", &bool_tmp))
		{
			App->renderer3D->draw_normals = bool_tmp;
		}
	}
}