#include "ComponentCamera.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleGOManager.h"
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
		float3 rot = rotation.ToEulerXYZ();
		rot = RadToDeg(rot);

		if(ImGui::DragFloat3("Rotation", rot.ptr()))
		{
			Rotate(rot);
		}
		bool bool_tmp = false;
		if (ImGui::Checkbox("Main camera", &main_cam))
		{
			if (main_cam)
			{
				SetAsMainCamera(main_cam);
			}
		}
		
		ImGui::Checkbox("Frustum culling", &frustum_culling);

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

void ComponentCamera::OnTransform()
{
	//NOTE: Maybe give options if the user wants the camera to transform too
	//Not working as expected
	/*float4x4 transform = go->GetTransform();
	float4x4 previous = ((ComponentTransform*)go->GetComponent(COMP_TRANSFORM))->GetPreviousWorldTransform();

	cam.Transform(previous.Inverted() * transform);*/
	


}

void ComponentCamera::Transform(const float4x4& trans)
{
	//NOTE: for now, it will be a global transformation
	cam.Transform(previous_transform.Inverted());
	cam.Transform(trans);

	previous_transform = trans;
}

void ComponentCamera::Rotate(const float3 r)
{
	//Note: polish this
	float3 rot = math::DegToRad(r);

	rotation = rotation.FromEulerXYZ(rot.x, rot.y, rot.z);

	float4x4 trans;
	float3 pos = float3::zero, scale = float3::one;
	trans = trans.FromTRS(pos, rotation, scale);

	Transform(trans);
}

void  ComponentCamera::Update()
{
	//Only on editor mode!!!! (should make a separate update for editor options)
	App->renderer3D->DrawFrustrum(cam);

	float3 up = cam.Up();
	float3 pos = cam.Pos();
	float3 front = cam.Front();

	float n = cam.NearPlaneDistance();
	float f = cam.FarPlaneDistance();

	float3 npos = pos + (front * n);

	float length = (f - n) / 4.0f;

	LineSegment up_line(npos, npos + (up*length));
	LineSegment front_line(npos, npos + (front * length));

	App->renderer3D->DrawLineSegment(up_line);
	App->renderer3D->DrawLineSegment(front_line);
}

void ComponentCamera::InitFrustrum()
{
	cam.SetKind(FrustumSpaceGL, FrustumRightHanded);

	//NOTE: testing
	cam.SetFrame(float3::zero, { 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0 });
	cam.SetViewPlaneDistances(1.0, 10);
	cam.SetPerspective(90, 90);

	transform = previous_transform = float4x4::identity;
}

void ComponentCamera::SetAspectRatio(float ratio)
{
	cam.SetVerticalFovAndAspectRatio(GetVerticalFOV(), ratio);
}

void ComponentCamera::SetVerticalFOV(float fov)
{
	cam.SetVerticalFovAndAspectRatio(fov, GetAspectRatio());
}

void ComponentCamera::SetAsMainCamera(bool is_main_camera)
{
	if (is_main_camera)
		App->go_manager->SetAsMainCamera(this);
	else
		App->go_manager->SetAsMainCamera(nullptr);
}

void ComponentCamera::SetCameraFrame(float3& pos, float3& front, float3& up)
{
	cam.SetFrame(pos, front, up);
}

void ComponentCamera::SetPerspective(float aspect_ratio, float fovy, float n, float f)
{
	cam.SetVerticalFovAndAspectRatio(fovy, aspect_ratio);
	cam.SetViewPlaneDistances(n, f);
}

float ComponentCamera::GetAspectRatio() const
{
	return cam.AspectRatio();
}

float ComponentCamera::GetVerticalFOV() const
{
	return cam.VerticalFov();
}

float* ComponentCamera::GetViewMatrix() const
{
	float4x4 mat = cam.ViewMatrix();
	mat.Transpose();
	return (float*) mat.v;
}

float* ComponentCamera::GetProjectionMatrix() const
{
	float4x4 mat = cam.ProjectionMatrix();
	mat.Transpose();
	return (float*)mat.v;
}

void ComponentCamera::LookAt(const float3& ref) 
{
	float3 dir = ref - cam.Pos();

	float4x4 rotation = float4x4::LookAt(cam.Front(), dir.Normalized(), cam.Up(), float3::unitY);

	cam.SetFront(rotation.MulDir(cam.Front()).Normalized());
	cam.SetUp(rotation.MulDir(cam.Up()).Normalized());
}

bool ComponentCamera::IsMainCamera() const
{
	return main_cam;
}

bool ComponentCamera::IntersectAABB(const AABB& box) const
{
	float3 frustum_center = cam.CenterPoint();

	for (uint i = 0; i < 6; i++)
	{
		Plane plane = cam.GetPlane(i);
		bool go_inside = false;

		for (uint j = 0; j < 8; j++)
		{
			float3 aabb_point = box.CornerPoint(j);

			if (plane.AreOnSameSide(frustum_center, aabb_point))
			{
				go_inside = true;
				break;
			}
		}

		if (!go_inside)
		{
			return false;
		}

	}

	return true;
}

bool ComponentCamera::CullGameObject(const GameObject* go) const
{
	//NOTE: if needed, this function will have to be optimized
	return IntersectAABB(go->bb_axis);

}
