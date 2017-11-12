#include "ModuleSceneEdit.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ComponentCamera.h"

#include "OpenGl.h"
#include "Math.h"

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

ModuleSceneEdit::ModuleSceneEdit(bool start_enabled): Module(start_enabled)
{
	name = "scene editor";
}

ModuleSceneEdit::~ModuleSceneEdit()
{

}

//-----------------------------------

bool ModuleSceneEdit::Start()
{
	wplane = new PWiredPlane(0,1,0,0);
	ex_cube = new PCube(2.0f, 2.0f, 2.0f);
	ex_cylinder = new PCylinder(1.0f, 3.0f);
	ex_sphere = new PSphere(1.0f);
	ex_plane = new PPlane(2.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	world_axis = new PAxis();

	root_GO = AddGameObject("Root");

	return true;
}

bool ModuleSceneEdit::CleanUp()
{
	delete wplane;
	delete ex_cube;
	delete ex_cylinder;
	delete ex_sphere;
	delete ex_plane;
	delete world_axis;

	//Delete all game_objects by deleting the root (recursive destructor)
	//NOTE: wich is better, recursive destructor (more secure) or vector delete iteration (more fast)?
	RELEASE(root_GO);

	game_objects.clear();

	return true;
}

//-----------------------------------

update_status ModuleSceneEdit::PreUpdate(float dt)
{
	if (main_camera && main_camera->frustum_culling)
		FrustumCulling();

	return UPDATE_CONTINUE;
}

update_status ModuleSceneEdit::Update(float dt)
{
	for (uint i = 0, size = game_objects.size(); i < size; i++)
	{
		game_objects[i]->Update();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneEdit::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

//------------------------------------

void ModuleSceneEdit::Draw()
{

	
	wplane->Render();

	glLineWidth(2.0f);

	world_axis->InnerRender();

}
//NOTE: do this here or in GO update?
void ModuleSceneEdit::FrustumCulling()
{
	for (uint i = 0, size = game_objects.size(); i < size; i++)
	{
		game_objects[i]->to_draw = main_camera->CullGameObject(game_objects[i]);
	}
}

GameObject* ModuleSceneEdit::AddGameObject(const char* name, GameObject* parent)
{
	GameObject* GO = new GameObject(name, parent);

	game_objects.push_back(GO);

	return GO;
}

void ModuleSceneEdit::SetAsMainCamera(ComponentCamera* cam)
{
	main_camera = cam;
}

bool ModuleSceneEdit::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	return ret;
}

bool ModuleSceneEdit::SaveConfig(JSON_Object* data) const
{
	bool ret = true;

	return ret;
}