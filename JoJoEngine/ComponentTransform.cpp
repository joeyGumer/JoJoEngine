#include "ComponentTransform.h"
#include "Imgui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* g) : Component(COMP_TRANSFORM, g)
{

}
ComponentTransform::~ComponentTransform()
{

}

void ComponentTransform::Update()
{

}

void ComponentTransform::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		//Position
		ImGui::Text("Position: ");
		ImGui::SameLine();

		if (ImGui::DragFloat3("###position", position.ptr(), 1.0f))
		{
			SetPosition(position);
		}

		float3 rot = rotation.ToEulerXYZ();
		rot = math::RadToDeg(rot);

		//Rotation
		ImGui::Text("Rotation :");
		ImGui::SameLine();
		if (ImGui::DragFloat3("###rotation", rot.ptr(), 1.0f))
		{
			SetRotation(rot);
		}

		//Scale
		ImGui::Text("Scale :");
		ImGui::SameLine();
		if (ImGui::DragFloat3("###scale", scale.ptr(), 1.0f))
		{
			SetScale(scale);
		}

		float4x4 t = local_transform;

		ImGui::Text("Local transform: ");
		ImGui::Text(" %.3f  %.3f  %.3f  %.3f", t.v[0][0], t.v[0][1], t.v[0][2], t.v[0][3]);
		ImGui::Text(" %.3f  %.3f  %.3f  %.3f", t.v[1][0], t.v[1][1], t.v[1][2], t.v[1][3]);
		ImGui::Text(" %.3f  %.3f  %.3f  %.3f", t.v[2][0], t.v[2][1], t.v[2][2], t.v[2][3]);
		ImGui::Text(" %.3f  %.3f  %.3f  %.3f", t.v[3][0], t.v[3][1], t.v[3][2], t.v[3][3]);
	}


}

float4x4 ComponentTransform::SetTransform(float3 &pos, Quat &rot, float3 &s)
{
	position = pos;
	rotation = rot;
	scale = s;

	return local_transform = local_transform.FromTRS(pos, rot, s);
}

void ComponentTransform::SetTransform(float4x4 &mat)
{
	local_transform = mat;
	local_transform.Decompose(position, rotation, scale);
}

void ComponentTransform::SetPosition(float3 &p)
{
	position = p;

	local_transform.SetTranslatePart(p);
}

void ComponentTransform::SetRotation(float3 &r)
{
	math::DegToRad(r);

	rotation = rotation.FromEulerXYZ(r.x, r.y, r.z);

	local_transform.SetRotatePart(rotation);
}
void ComponentTransform::SetScale(float3 &s)
{
	scale = s;

	//NOTE: Theres Scale() but not sure it will overlap the local_transform
	local_transform.FromTRS(position, rotation, scale);
}