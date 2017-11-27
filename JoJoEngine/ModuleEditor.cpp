#include "Application.h"

#include "Globals.h"
#include "ModuleEditor.h"
#include "EditorWindow.h"
#include "WinConfiguration.h"
#include "WinProperties.h"
#include "WinHierarchy.h"
#include "WinConsole.h"
#include "WinGizmo.h"
#include "ImGuiDemo.h"


#include "JSON\parson.h"
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
				configuration->OpenCloseWindow();
			}

			if (ImGui::MenuItem("Properties", NULL))
			{
				properties->OpenCloseWindow();
			}

			if (ImGui::MenuItem("Console", NULL))
			{
				console->OpenCloseWindow();
			}

			if (editor_active)
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
				demo->OpenCloseWindow();

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

const bool ModuleEditor::IsActive() const
{
	return editor_active;
}


void ModuleEditor::Draw() const
{
	if (editor_active)
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


void ModuleEditor::AboutUs() const
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
	ImGui::Text("PhysicsFS 2.0");
	ImGui::Text("Glut 3.7");
	ImGui::Text("Glew 7.0");
	ImGui::Text("Assimp 3.1.1");
	ImGui::Text("Devil 1.7.8");

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
	editor_active = !editor_active;

	for (uint i = 0; i < editor_windows.size(); i++)
	{
		if (!editor_active && editor_windows[i]->is_open)
		{
			editor_windows[i]->want_to_be_open = true;
			editor_windows[i]->is_open = false;
		}
		else if (editor_active && editor_windows[i]->want_to_be_open)
		{
			editor_windows[i]->want_to_be_open = false;
			editor_windows[i]->is_open = true;
		}
	}
}

void ModuleEditor::CreateWindows()
{
	//Creating all editor windows
	configuration = new WinConfiguration();
	demo = new ImGuiDemo();
	properties = new WinProperties();
	hierarchy = new WinHierarchy();
	gizmo = new WinGizmo();

	//Adding all editor windows to the vector (order is important)
	AddWindow(configuration);
	AddWindow(properties);
	AddWindow(hierarchy);
	AddWindow(demo);
	AddWindow(console);
	AddWindow(gizmo);
}

bool ModuleEditor::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	//This shouldn't be here, but it's needed to load "is_open" booleans
	CreateWindows();

	hierarchy->is_open = json_object_get_boolean(data, "hierarchy_open");
	configuration->is_open = json_object_get_boolean(data, "configuration_open");
	properties->is_open = json_object_get_boolean(data, "properties_open");
	demo->is_open = json_object_get_boolean(data, "demo_open");
	console->is_open = json_object_get_boolean(data, "console_open");
	editor_active = json_object_get_boolean(data, "windows_active");

	return ret;
}

bool ModuleEditor::SaveConfig(JSON_Object* data) const
{
	bool ret = true;
	
	json_object_set_boolean(data, "hierarchy_open", hierarchy->is_open);
	json_object_set_boolean(data, "configuration_open", configuration->is_open);
	json_object_set_boolean(data, "console_open", console->is_open);
	json_object_set_boolean(data, "demo_open", demo->is_open);
	json_object_set_boolean(data, "properties_open", properties->is_open);
	json_object_set_boolean(data, "editor_active", editor_active);

	return ret;
}