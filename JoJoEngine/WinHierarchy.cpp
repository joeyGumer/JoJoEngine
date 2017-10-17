#include "WinHierarchy.h"
#include "Application.h"
#include "ModuleSceneEdit.h"
#include "GameObject.h"
#include "Globals.h"

#include "Imgui/imgui.h"

WinHierarchy::WinHierarchy() : EditorWindow()
{
}

WinHierarchy::~WinHierarchy()
{


}

void WinHierarchy::Start()
{

}

void WinHierarchy::CleanUp()
{

}

void WinHierarchy::Update()
{
	ImGui::Begin("Hierarchy", &is_open);
	
	GameObject* root = App->level->root_GO;

	//Iterate as tree is faster than iterating the vector?
	ShowGO(root);

	ImGui::End();
}


void WinHierarchy::ShowGO(GameObject* go)
{
	if(ImGui::TreeNodeEx(go->name.c_str()))
	{
		for (uint i = 0, size = go->children.size(); i < size; i++)
		{
			//NOTE: have to polish so different nodes with same name doesn't interact at the same time
			ShowGO(go->children[i]);
		}

		ImGui::TreePop();

	}
}

