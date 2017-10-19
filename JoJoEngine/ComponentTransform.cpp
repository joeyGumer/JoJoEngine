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
		float3 position;
		Quat rotation;
		float3 scale; 

		local_transform.Decompose(position, rotation, scale);

		float3 rot = rotation.ToEulerXYZ();

		ImGui::Text("Position :");
		ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", position.x, position.y, position.z);
		ImGui::Text("Rotation :");
		ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", rot.x, rot.y, rot.z);
		ImGui::Text("Scale :");
		ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", scale.x, scale.y, scale.z);
	}
}

float4x4 ComponentTransform::SetTransform(float3 &pos, Quat &rot, float3 &s)
{
	return local_transform = local_transform.FromTRS(pos, rot, s);
}

void ComponentTransform::SetTransform(float4x4 &mat)
{
	local_transform = mat;
}