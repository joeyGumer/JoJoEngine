#include "ModuleSceneEdit.h"
#include "Primitive.h"
#include "GameObject.h"
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
	return true;
}

//-----------------------------------

update_status ModuleSceneEdit::PreUpdate(float dt)
{
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

GameObject* ModuleSceneEdit::AddGameObject()
{
	GameObject* GO = new GameObject();

	game_objects.push_back(GO);

	return GO;
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