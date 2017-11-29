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


		float4x4 transform = c_transform->GetWorldTransform();
		transform.Transpose();

		float* view = ((float4x4)App->camera->cam->cam.ViewMatrix()).Transposed().ptr();
		float* projection = App->camera->cam->cam.ProjectionMatrix().Transposed().ptr();

		ImGuizmo::SetRect(0, 0, App->window->GetWidth(), App->window->GetHeight());

		if(operation != ImGuizmo::SCALE)
			ImGuizmo::Manipulate(view, projection, operation, mode, transform.ptr());
		else
			ImGuizmo::Manipulate(view, projection, operation, ImGuizmo::LOCAL, transform.ptr());


		if (ImGuizmo::IsUsing())
		{

			if (ComponentTransform* parent_transform = ((ComponentTransform*)go_select->GetParent()->GetComponent(COMP_TRANSFORM)))
			{
				transform = parent_transform->GetWorldTransform().Inverted() * transform;
			}

			transform.Transpose();
			c_transform->SetTransform(transform);
		}
	}

}
void WinGizmo::CleanUp()
{

}