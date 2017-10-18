#include "ComponentTransform.h"
#include "Imgui/imgui.h"

ComponentTransform::ComponentTransform() : Component(COMP_TRANSFORM)
{
	local_transform.SetIdentity();
}
ComponentTransform::ComponentTransform(float4x4 &mat) : Component(COMP_TRANSFORM), local_transform(mat)
{
	local_transform.Decompose(position, rotation, scale);
}
ComponentTransform::ComponentTransform(float3 &p, Quat &r, float3 &s ) : Component(COMP_TRANSFORM), position(p), rotation(r), scale(s)
{
	local_transform = local_transform.FromTRS(position, rotation, scale);
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
		float3 rot = rotation.ToEulerXYZ();

		ImGui::Text("Position :");
		ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", position.x, position.y, position.z);
		ImGui::Text("Rotation :");
		ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", rot.x, rot.y, rot.z);
		ImGui::Text("Scale :");
		ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", scale.x, scale.y, scale.z);
	}
}