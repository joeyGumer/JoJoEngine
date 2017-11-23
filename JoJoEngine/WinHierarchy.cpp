#include "WinHierarchy.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "WinProperties.h"
#include "Application.h"
#include "ModuleGOManager.h"
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
	
	GameObject* root = App->go_manager->root_GO;

	//Iterate as tree is faster than iterating the vector?ç
	//Is the hierarchy so this is fine
	//NOTE: not show root as GO
	go_id = 0;

	for (uint i = 0; i < root->children.size(); i++)
	{
		ShowGO(root->children[i]);
	}

	ImGui::End();
}


void WinHierarchy::ShowGO(GameObject* go)
{
	//NOTE: Too slow, have to find another way
	sprintf_s(label, 100, "%s##id_%d", go->name.c_str(), go_id++);
	
	//NOTE: EDIT FANCY FUNCTIONALITY
	if(ImGui::TreeNodeEx(label, 0))
	{
		if (ImGui::IsItemClicked())
		{
			App->go_manager->SetGoSelected(go);
		}



		for (uint i = 0, size = go->children.size(); i < size; i++)
		{
			//NOTE: have to polish so different nodes with same name doesn't interact at the same time
			ShowGO(go->children[i]);

		}

		ImGui::TreePop();

	}
}

