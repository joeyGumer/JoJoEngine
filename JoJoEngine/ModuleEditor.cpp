#include "Application.h"
#include "ModuleEditor.h"
#include "PhysBody3D.h"

#include "Imgui\imgui.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{}

// Load assets
bool ModuleEditor::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	

	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOG("Unloading Intro scene");

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

	return UPDATE_CONTINUE;
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


