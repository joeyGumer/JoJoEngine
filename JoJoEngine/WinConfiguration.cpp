#include "WinConfiguration.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "OpenGl.h"
#include "Imgui/imgui.h"

WinConfiguration::WinConfiguration() : EditorWindow()
{

}

WinConfiguration::~WinConfiguration()
{

}

void WinConfiguration::Start()
{
	slider_fps = 60;
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

		TabApplication();
		TabWindow();	
		TabRenderer();
		TabHardware();

		ImGui::End();
	}
}

void WinConfiguration::TabApplication()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Personalization:");
		ImGui::Spacing();

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

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Performance:");
		ImGui::Spacing();
		//Performance
		if (ImGui::SliderInt("Max FPS", &slider_fps, 0, 60, NULL))
			App->SetMaxFPS(slider_fps);

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

void WinConfiguration::TabHardware()
{
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "CPU:");
		ImGui::Spacing();
		ImGui::TextWrapped("CPUs: %i Cores", SDL_GetCPUCount());
		ImGui::TextWrapped("Memory Ram: %i GB", (SDL_GetSystemRAM() / 1000));
		ImGui::TextWrapped("CPU Cache: %i bits", SDL_GetCPUCacheLineSize());

		string tmp;
		if(SDL_Has3DNow()) tmp += "3DNow, ";
		if(SDL_HasAltiVec()) tmp += "AltiVec, ";
		if(SDL_HasAVX()) tmp += "AVX, ";		
		if(SDL_HasMMX()) tmp += "MMX, ";
		if(SDL_HasRDTSC()) tmp += "RDTSC, ";
		if(SDL_HasSSE()) tmp += "SSE, ";
		if(SDL_HasSSE2()) tmp += "SSE2, ";
		if(SDL_HasSSE3()) tmp += "SSE3, ";
		if(SDL_HasSSE41()) tmp += "SSE41, ";
		if(SDL_HasSSE42()) tmp += "SSE42, ";
		ImGui::TextWrapped("Caps: %s", tmp.c_str());		

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "GPU:");
		ImGui::Spacing();
		ImGui::TextWrapped("Brand: %s", glGetString(GL_VENDOR));
		ImGui::TextWrapped("GPU: %s", glGetString(GL_RENDERER));
		ImGui::TextWrapped("GL Version: %s", glGetString(GL_VERSION));
		ImGui::TextWrapped("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
}