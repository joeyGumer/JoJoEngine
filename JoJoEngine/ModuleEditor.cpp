#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "PhysBody3D.h"
#include "EditorWindow.h"
#include "WinConfiguration.h"

#include "Brofiler/Brofiler.h"
#include "Imgui\imgui.h"
#include <vector>

using namespace std;


ModuleEditor::ModuleEditor( bool start_enabled) : Module(start_enabled)
{
	name = "editor";

	//Creating all editor windows
	configuration = new WinConfiguration();

	//Adding all editor windows to the vector (order is important)
	AddWindow(configuration);
}


ModuleEditor::~ModuleEditor()
{}

// Load assets
bool ModuleEditor::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//NOTE: create at Start or at Constructor
	

	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOG("Unloading Intro scene");

	//Delete all windows
	for (int i = editor_windows.size()-1; i >= 0; i--)
	{
		if (editor_windows[i] != nullptr)
			delete editor_windows[i];
	}

	editor_windows.clear();

	return true;
}

// Update
update_status ModuleEditor::Update(float dt)
{
	BROFILER_CATEGORY("ModuleEditor::Update", Profiler::Color::AliceBlue);
	//Create the menu bar
	if(ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", NULL))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Configuration", NULL))
			{
				configuration->is_open = !configuration->is_open;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Demo", NULL))
				show_demo = !show_demo;

			if (ImGui::MenuItem("Download latest version"))
				ShellExecuteA(NULL, "open", "https://github.com/joeyGumer/JoJoEngine/releases", NULL, NULL, NULL);

			if (ImGui::MenuItem("Report a bug"))
				ShellExecuteA(NULL, "open", "https://github.com/joeyGumer/JoJoEngine/issues", NULL, NULL, NULL);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About us"))
		{
			AboutUs();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	
	if(show_demo)
		ImGui::ShowTestWindow();

	//Iterate all editor windows
	/*for (uint i = 0; i < editor_windows.size(); i++)
	{
		editor_windows[i]->Update();
	}*/

	return UPDATE_CONTINUE;
}


void ModuleEditor::Draw() const
{
	//Iterate all editor windows
	for (uint i = 0; i < editor_windows.size(); i++)
	{
		editor_windows[i]->Update();
	}

	//Render all ImGui elements defined
	ImGui::Render();
}

void ModuleEditor::AboutUs()
{
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "JoJo Engine");


	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("JoJo Engine is a college project about make basic 3D videogame \nengine from scratch. (CITM-UPC, 3rd course Game Engines subject)");

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Authors:"); ImGui::Spacing();
	ImGui::Text("Josep Atencia");
	ImGui::Text("Jordi Nunes");

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Libraries Used:"); ImGui::Spacing();
	ImGui::Text(App->GetSDLVersion().c_str());
	ImGui::Text("OpenGL 3.1");
	ImGui::Text("ImGui 1.50");
	ImGui::Text("MathGeoLib 1.5");
	ImGui::Text("Parson");
}

void ModuleEditor::AddWindow(EditorWindow* win)
{
	editor_windows.push_back(win);
}

bool ModuleEditor::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	return ret;
}

bool ModuleEditor::SaveConfig(JSON_Object* data)
{
	bool ret = true;

	return ret;
}