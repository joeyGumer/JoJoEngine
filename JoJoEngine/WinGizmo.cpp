#include "WinGizmo.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleGOManager.h"
#include "ModuleWindow.h"

#include "GameObject.h"
#include "ComponentTransform.h"

#include "ImGuizmo/ImGuizmo.h"
#include "Imgui/imgui.h"


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

	ImGuizmo::Enable(true);

	GameObject* go_select = App->go_manager->GetGoSelected();

	
	if (go_select)
	{
		ComponentTransform* c_transform = (ComponentTransform*)go_select->GetComponent(COMP_TRANSFORM);

		ImGuizmo::OPERATION operation = (ImGuizmo::OPERATION)c_transform->operation;
		ImGuizmo::MODE mode = (ImGuizmo::MODE)c_transform->mode;


		float4x4 transform = c_transform->GetLocalTransform();
		transform.Transpose();

		float* v_trans = (float*)transform.v;


		float* view = ((float4x4)App->camera->cam->cam.ViewMatrix()).Transposed().ptr();
		float* projection = App->camera->cam->cam.ProjectionMatrix().Transposed().ptr();

		ImGuizmo::SetRect(0, 0, App->window->GetWidth(), App->window->GetHeight());
		ImGuizmo::DrawCube(App->camera->GetViewMatrix(), App->camera->GetProjectionMatrix(), v_trans);


		ImGuizmo::Manipulate(view, projection, operation, mode, v_trans);
	}

}
void WinGizmo::CleanUp()
{

}