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
	//FPS Graphs
	slider_fps = 60;
	fps.StartBar(1000);
	ms.StartBar(1);

	//Hardware specs
	if (SDL_Has3DNow()) caps += "3DNow, ";
	if (SDL_HasAltiVec()) caps += "AltiVec, ";
	if (SDL_HasAVX()) caps += "AVX, ";
	if (SDL_HasMMX()) caps += "MMX, ";
	if (SDL_HasRDTSC()) caps += "RDTSC, ";
	if (SDL_HasSSE()) caps += "SSE, ";
	if (SDL_HasSSE2()) caps += "SSE2, ";
	if (SDL_HasSSE3()) caps += "SSE3, ";
	if (SDL_HasSSE41()) caps += "SSE41, ";
	if (SDL_HasSSE42()) caps += "SSE42, ";

	//VRAM
	glGetIntegerv(GPU_TOTAL_AVAILABLE_MEM, &total_vram);
	glGetIntegerv(GPU_CURRENT_AVAILABLE_MEM, &available_vram);
	glGetIntegerv(GPU_USAGE_MEM, &usage_vram);
	total_vram /= 1000;
	available_vram /= 1000;
	usage_vram /= 1000;
}

void WinConfiguration::CleanUp()
{
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

		bool_tmp = App->window->GetVsync();
		if (ImGui::Checkbox("Vsync", &bool_tmp))
		{
			App->window->SetVsync(bool_tmp);
		}

		int_tmp = App->window->GetWindowMode();
		if(ImGui::RadioButton("Resizable", &int_tmp, RESIZABLE))
			App->window->SetWindowMode(int_tmp);
		ImGui::SameLine();
		if(ImGui::RadioButton("Fullscreen Desktop", &int_tmp, FULL_DESKTOP))
			App->window->SetWindowMode(int_tmp);
		ImGui::SameLine();
		if(ImGui::RadioButton("Fullscreen", &int_tmp, FULLSCREEN))
			App->window->SetWindowMode(int_tmp);

		if (App->window->GetWindowMode() == RESIZABLE)
		{
			int_tmp = App->window->GetWidth();
			if (ImGui::SliderInt("Width", &int_tmp, 0, 1920))
			{
				App->window->SetWidth(int_tmp);
			}

			int_tmp = App->window->GetHeight();
			if (ImGui::SliderInt("Height", &int_tmp, 0, 1080))
			{
				App->window->SetHeight(int_tmp);
			}
		}
	}
}

void WinConfiguration::TabRenderer()
{
	if (ImGui::CollapsingHeader("Render"))
	{
		bool bool_tmp;

		if (ImGui::TreeNodeEx("Geometry"))
		{
			bool_tmp = App->renderer3D->draw_normals;
			if (ImGui::Checkbox("Draw Normals", &bool_tmp))
			{
				App->renderer3D->draw_normals = bool_tmp;
			}

			bool_tmp = App->renderer3D->draw_wireframe;
			if (ImGui::Checkbox("Draw wireframe", &bool_tmp))
			{
				App->renderer3D->draw_wireframe = bool_tmp;
			}

			bool_tmp = App->renderer3D->draw_meshes;
			if (ImGui::Checkbox("Draw mesh", &bool_tmp))
			{
				App->renderer3D->draw_meshes = bool_tmp;
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Textures"))
		{
			bool_tmp = App->renderer3D->draw_textures;
			if (ImGui::Checkbox("Draw textures", &bool_tmp))
			{
				App->renderer3D->EnableTextures(bool_tmp);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Lights"))
		{


			ImGui::TreePop();
		}
	}
}

void WinConfiguration::TabHardware()
{
	if (ImGui::CollapsingHeader("Hardware"))
	{
		//CPU
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "CPU:");
		ImGui::Spacing();
		ImGui::TextWrapped("CPUs: %i Cores", SDL_GetCPUCount());
		ImGui::TextWrapped("Memory Ram: %i GB", (SDL_GetSystemRAM() / 1000));
		ImGui::TextWrapped("CPU Cache: %i bits", SDL_GetCPUCacheLineSize());
		ImGui::TextWrapped("Caps: %s", caps.c_str());		

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		//GPU
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "GPU:");
		ImGui::Spacing();
		ImGui::TextWrapped("Brand: %s", glGetString(GL_VENDOR));
		ImGui::TextWrapped("GPU: %s", glGetString(GL_RENDERER));
		ImGui::TextWrapped("GL Version: %s", glGetString(GL_VERSION));
		ImGui::TextWrapped("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		
		//VRAM		
		ImGui::TextWrapped("VRAM Budget: %i MB", total_vram);
		ImGui::TextWrapped("VRAM Avaliable: %i MB", available_vram);
		ImGui::TextWrapped("VRAM Usage: %i MB", usage_vram);
	}
}