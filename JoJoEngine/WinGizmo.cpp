#include "WinGizmo.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleGOManager.h"

#include "GameObject.h"

#include "ImGuizmo/ImGuizmo.h"


WinGizmo::WinGizmo() : EditorWindow()
{

}
WinGizmo::~WinGizmo()
{

}


void WinGizmo::Start()
{

}
void WinGizmo::Update()
{
	ImGuizmo::BeginFrame();

	GameObject* go_select = App->go_manager->GetGoSelected();

	if (go_select)
	{
		//Note: Check why it doesn't show comparing to  project 3 gizmos
		ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE mode = ImGuizmo::WORLD;

		float4x4 transform = go_select->GetTransform();
		transform.Transpose();
		float* v_trans = (float*)transform.v;


		ImGuizmo::SetDrawlist();

		ImGuizmo::DrawCube(App->camera->GetViewMatrix(), App->camera->GetProjectionMatrix(), v_trans);

		ImGuizmo::Manipulate(App->camera->GetViewMatrix(), App->camera->GetProjectionMatrix(), operation, mode, v_trans);
	}
	
}
void WinGizmo::CleanUp()
{

}