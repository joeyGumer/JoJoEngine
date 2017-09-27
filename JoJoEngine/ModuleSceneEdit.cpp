#include "ModuleSceneEdit.h"

ModuleSceneEdit::ModuleSceneEdit(bool start_enabled): Module(start_enabled)
{

}

ModuleSceneEdit::~ModuleSceneEdit()
{

}

//-----------------------------------

bool ModuleSceneEdit::Start()
{
	return true;
}

bool ModuleSceneEdit::CleanUp()
{
	return true;
}

//-----------------------------------

update_status ModuleSceneEdit::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEdit::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEdit::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

//------------------------------------

void ModuleSceneEdit::Draw()
{

}