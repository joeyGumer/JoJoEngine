#include "ModuleGOManager.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"

#include "GameObject.h"
#include "ComponentCamera.h"


ModuleGOManager::ModuleGOManager(bool start_enabled) : Module(start_enabled)
{
	name = "go_manager";
}

ModuleGOManager::~ModuleGOManager()
{

}


bool ModuleGOManager::Start()
{
	root_GO = AddGameObject("Root");

	return true;
}
bool ModuleGOManager::CleanUp()
{
	RELEASE(root_GO);

	game_objects.clear();

	return true;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	//Camera Focus on Geometry
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_UP)
		FocusGameObject();

	return UPDATE_CONTINUE;
}
update_status ModuleGOManager::Update(float dt)
{
	if (main_camera && main_camera->frustum_culling)
		FrustumCulling();

	return UPDATE_CONTINUE;
}
update_status ModuleGOManager::PostUpdate(float dt)
{
	for (uint i = 0, size = game_objects.size(); i < size; i++)
	{
		game_objects[i]->Update();
	}

	return UPDATE_CONTINUE;
}

GameObject* ModuleGOManager::AddGameObject(const char* name, GameObject* parent)
{
	GameObject* GO = new GameObject(name, parent);

	game_objects.push_back(GO);

	return GO;
}

void ModuleGOManager::SetAsMainCamera(ComponentCamera* cam)
{
	main_camera = cam;
}
void ModuleGOManager::SetGoSelected(GameObject* s_go)
{
	selected_GO = s_go;
}

GameObject*  ModuleGOManager::GetGoSelected()const
{
	return selected_GO;
}
void ModuleGOManager::FocusGameObject() const
{
	bool degenerate = selected_GO->bb_axis.IsDegenerate();
	if (selected_GO && !degenerate)
		App->camera->CenterCameraOnGeometry(selected_GO->bb_axis);
}

void ModuleGOManager::FrustumCulling()
{
	for (uint i = 0, size = game_objects.size(); i < size; i++)
	{
		game_objects[i]->to_draw = main_camera->CullGameObject(game_objects[i]);
	}
}

