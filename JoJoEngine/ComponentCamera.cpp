#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Imgui\imgui.h"


ComponentCamera::ComponentCamera(GameObject* go): Component(COMP_CAMERA, go)
{
	InitFrustrum();
}
ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera"))
	{		
		float2 tmp_float2 = { cam.NearPlaneDistance(), cam.FarPlaneDistance() };
		float tmp_float = 0.0f;
		if(ImGui::DragFloat2("Near / Far ", tmp_float2.ptr(), 0.1f))
		{
			cam.SetViewPlaneDistances(tmp_float2.x, tmp_float2.y);
		}

		ImGui::Text("Vertical FOV");
		ImGui::SameLine();

		tmp_float = GetVerticalFOV();
		if(ImGui::DragFloat("###v_fov", &tmp_float, 0.1f))
		{
			SetVerticalFOV(tmp_float);
		}

		tmp_float = GetAspectRatio();
		if (ImGui::DragFloat("###a_ratio", &tmp_float, 0.1f))
		{
			SetAspectRatio(tmp_float);
		}
	}
}

void  ComponentCamera::Update()
{
	App->renderer3D->DrawFrustrum(cam);
}

void ComponentCamera::InitFrustrum()
{
	//NOTE: testing
	cam.SetFrame(float3::zero, { 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0 });
	cam.SetViewPlaneDistances(1.0, 10);
	cam.SetPerspective(90, 90);
}

void ComponentCamera::SetAspectRatio(float ratio)
{
	cam.SetVerticalFovAndAspectRatio(GetVerticalFOV(), ratio);
}

void ComponentCamera::SetVerticalFOV(float fov)
{
	cam.SetVerticalFovAndAspectRatio(fov, GetAspectRatio());
}

float ComponentCamera::GetAspectRatio() const
{
	return cam.AspectRatio();
}

float ComponentCamera::GetVerticalFOV() const
{
	return cam.VerticalFov();
}
