#include "WinConfiguration.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleGOManager.h"

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
	total_vram /= 1000;
	glGetIntegerv(GPU_CURRENT_AVAILABLE_MEM, &available_vram);
	available_vram /= 1000;
	initial_available_vram = available_vram;
	usage_vram = initial_available_vram - available_vram;
}

void WinConfiguration::CleanUp()
{
}

void WinConfiguration::Update()
{
	fps.FillBar(App->GetFPS());
	ms.FillBar(App->GetMs());

	glGetIntegerv(GPU_CURRENT_AVAILABLE_MEM, &available_vram);
	available_vram /= 1000;
	usage_vram = initial_available_vram - available_vram;

	if (is_open)
	{
		ImGui::Begin("Configuration", &is_open);

		ImGui::PushItemWidth(-140);

		TabApplication();
		TabWindow();	
		TabRenderer();
		TabCamera();
		TabInput();
		TabGoManager();
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

			bool_tmp = App->renderer3D->GetEnableDepth();
			if (ImGui::Checkbox("Enable Depth", &bool_tmp))
			{
				App->renderer3D->SetEnableDepth(bool_tmp);
			}

			bool_tmp = App->renderer3D->GetEnableCullFace();
			if (ImGui::Checkbox("Enable Face Culling", &bool_tmp))
			{
				App->renderer3D->SetEnableCullFace(bool_tmp);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Textures"))
		{
			bool_tmp = App->renderer3D->GetEnableTextures();
			if (ImGui::Checkbox("Draw textures", &bool_tmp))
			{
				App->renderer3D->SetEnableTextures(bool_tmp);
			}

			bool_tmp = App->renderer3D->GetEnableColorMaterial();
			if (ImGui::Checkbox("Enable Color Material", &bool_tmp))
			{
				App->renderer3D->SetEnableColorMaterial(bool_tmp);
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Lights"))
		{
			bool_tmp = App->renderer3D->GetEnableLight();
			if (ImGui::Checkbox("Enable Lightning", &bool_tmp))
			{
				App->renderer3D->SetEnableLight(bool_tmp);
			}
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


void WinConfiguration::TabCamera()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("X: %.3f   ", App->camera->Position.x);
		ImGui::SameLine();
		ImGui::Text("Y: %.3f   ", App->camera->Position.y);
		ImGui::SameLine();
		ImGui::Text("Z: %.3f", App->camera->Position.z);

		float tmp_float;

		tmp_float = App->camera->speed * (100 / App->camera->max_speed);
		ImGui::DragFloat("Camera Speed", &tmp_float, 1.0f, 0.0f, 100.0f);
		App->camera->speed = tmp_float / (100 / App->camera->max_speed);

		tmp_float = App->camera->wheel_speed * (100 / App->camera->max_wheel_speed);
		ImGui::DragFloat("Camera Zoom Speed", &tmp_float, 1.0f, 0.0f, 100.0f);
		App->camera->wheel_speed = tmp_float / (100 / App->camera->max_wheel_speed);

		tmp_float = App->camera->sensitivity * (100 / App->camera->max_sensitivity);
		ImGui::DragFloat("Camera Rotation Sensitivity", &tmp_float, 1.0f, 0.0f, 100.0f);
		App->camera->sensitivity = tmp_float / (100 / App->camera->max_sensitivity);
	}
}

void WinConfiguration::TabInput()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("X: %i   ", App->input->GetMouseX());
		ImGui::SameLine();
		ImGui::Text("Y: %i   ", App->input->GetMouseY());

		ImGui::Spacing();
		ImGui::Text("Number of joysticks: %i", App->input->GetNumberJoysticks());
	}
}

void WinConfiguration::TabGoManager()
{
	if (ImGui::CollapsingHeader("Go Manager"))
	{
		if (ImGui::Button("Create Quadtree"))
		{
			App->go_manager->FillQuadTree();
		}
		//Quadtree
		RecursiveDrawQuadtree(&App->go_manager->tree.root_node);
	}
}

void WinConfiguration::RecursiveDrawQuadtree(void* node)
{
	uint flags = 0;

	/*if (node->childs[0] == nullptr)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(node, flags, "QNode"))
	{
		for (list<GameObject*>::const_iterator it = node->objects.begin(); it != node->objects.end(); ++it)
			ImGui::Text("%s", (*it)->name.c_str());

		for (uint i = 0; i < 4; ++i)
			if (node->childs[i] != nullptr)
				RecursiveDrawQuadtree(node->childs[i]);

		ImGui::TreePop();
	}*/
}