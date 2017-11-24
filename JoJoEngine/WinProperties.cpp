#include "WinProperties.h"

#include "Application.h"
#include "ModuleGOManager.h"

#include "GameObject.h"
#include "Component.h"

#include "Imgui/imgui.h"

WinProperties::WinProperties()
{

}
WinProperties::~WinProperties()
{

}

void WinProperties::Start()
{

}

void WinProperties::CleanUp()
{

}

void WinProperties::Update()
{
	if (is_open)
	{
		ImGui::Begin("Properties", &is_open);

		//NOTE: change by event?
		go = App->go_manager->GetGoSelected();

		if (go)
		{


			if (go->name.capacity() < 100)
			{
				go->name.resize(100);
			}
			ImGui::InputText("", (char*)go->name.c_str(), 100);

			go->id_comp = 0;
			for (uint i = 0, size = go->components.size(); i < size; i++, go->id_comp++)
			{
				go->components[i]->OnEditor();
			}

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("comp");
			if (ImGui::BeginPopup("comp"))
			{
				TypeComp type = COMP_DEFAULT;
				
				if (ImGui::Selectable("Camera"))
				{
					type = COMP_CAMERA;
				}

				if (type != COMP_DEFAULT)
				{
					go->AddComponent(type);
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}
}

void WinProperties::SetGameObject(GameObject* g)
{
	go = g;
}