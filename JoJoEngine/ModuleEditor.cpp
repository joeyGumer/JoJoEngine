#include "Application.h"
#include "Console.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "PhysBody3D.h"
#include "EditorWindow.h"
#include "WinConfiguration.h"
#include "ImGuiDemo.h"

#include "Brofiler/Brofiler.h"
#include <vector>

using namespace std;


ModuleEditor::ModuleEditor( bool start_enabled) : Module(start_enabled)
{
	name = "editor";
}


ModuleEditor::~ModuleEditor()
{}

// Load assets
bool ModuleEditor::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//Creating all editor windows
	configuration = new WinConfiguration();
	demo = new ImGuiDemo();

	//Adding all editor windows to the vector (order is important)
	AddWindow(configuration);
	AddWindow(demo);
	AddWindow(console);

	for (uint i = 0; i < editor_windows.size(); i++)
	{
		editor_windows[i]->Start();
	}

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
		{
			if (editor_windows[i] != console)
			{
				editor_windows[i]->CleanUp();
				delete editor_windows[i];
			}
		}
	}
	editor_windows.clear();

	return true;
}

// Update
update_status ModuleEditor::Update(float dt)
{
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

			if (ImGui::MenuItem("Console", NULL))
			{
				console->is_open = !console->is_open;
			}

			if (windows_on)
			{
				if (ImGui::MenuItem("Turn Off windows", NULL))
					TurnOnOff();
			}
			else
			{
				if (ImGui::MenuItem("Turn On windows", NULL))
					TurnOnOff();
			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Demo", NULL))
				demo->is_open = !demo->is_open;

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

	return UPDATE_CONTINUE;
}


void ModuleEditor::Draw() const
{
	if (windows_on)
	{
		//Iterate all editor windows
		for (uint i = 0; i < editor_windows.size(); i++)
		{
			editor_windows[i]->Update();
		}
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

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::MenuItem("Take a look to the repository"))
		ShellExecuteA(NULL, "open", "https://github.com/joeyGumer/JoJoEngine", NULL, NULL, NULL);
}

void ModuleEditor::AddWindow(EditorWindow* win)
{
	editor_windows.push_back(win);
}

void ModuleEditor::TurnOnOff()
{
	for (uint i = 0; i < editor_windows.size(); i++)
	{
		if (editor_windows[i]->is_open)
		{
			windows_on = false;
			editor_windows[i]->want_to_be_open = true;
			editor_windows[i]->is_open = false;
		}
		else if (editor_windows[i]->want_to_be_open)
		{
			windows_on = true;
			editor_windows[i]->want_to_be_open = false;
			editor_windows[i]->is_open = true;
		}
	}
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