#include "ComponentTransform.h"

#include "GameObject.h"

#include "Globals.h"
#include "Imgui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* g) : Component(COMP_TRANSFORM, g)
{
	previous_world_transform.SetIdentity();
	world_transform.SetIdentity();
	local_transform.SetIdentity();

	CalculateWorldTransform();
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
		bool tmp_bool = go->IsStatic();
		if(ImGui::Checkbox("Static", &tmp_bool))
		{
			go->SetStatic(tmp_bool);
		}

		//Transform operation

		ImGui::Text("Operation:");
		tmp_bool = (operation == TRANSLATE);
		if (ImGui::Checkbox("Translation", &tmp_bool))
		{
			if (tmp_bool)
			{
				operation = TRANSLATE;
			}
		}

		ImGui::SameLine();
		tmp_bool = (operation == ROTATE);
		if (ImGui::Checkbox("Rotation", &tmp_bool))
		{
			if (tmp_bool)
			{
				operation = ROTATE;
			}
		}

		ImGui::SameLine();
		tmp_bool = (operation == SCALE);
		if (ImGui::Checkbox("Scale", &tmp_bool))
		{
			if (tmp_bool)
			{
				operation = SCALE;
			}
		}

		//Transform mode
		ImGui::Text("Mode:");
		tmp_bool = (mode == WORLD);
		if (ImGui::Checkbox("World", &tmp_bool))
		{
			if (tmp_bool)
			{
				mode = WORLD;
			}
		}

		tmp_bool = (mode == LOCAL);
		if (ImGui::Checkbox("Local", &tmp_bool))
		{
			if (tmp_bool)
			{
				mode = LOCAL;
			}
		}



		//Position
		ImGui::Text("Position: ");
		ImGui::SameLine();

		if (ImGui::DragFloat3("###position", position.ptr(), 0.1f))
		{
			SetPosition(position);
		}

		float3 rot = rotation.ToEulerXYZ();
		rot = math::RadToDeg(rot);

		//Rotation
		ImGui::Text("Rotation :");
		ImGui::SameLine();
		if (ImGui::DragFloat3("###rotation", rot.ptr(), 0.1f))
		{
			SetRotation(rot);
		}

		//Scale
		ImGui::Text("Scale :");
		ImGui::SameLine();
		if (ImGui::DragFloat3("###scale", scale.ptr(), 0.1f))
		{
			SetScale(scale);
		}

		float4x4 t = local_transform;

		ImGui::Text("Local transform: ");
		ImGui::Text(" %.3f  %.3f  %.3f  %.3f", t.v[0][0], t.v[0][1], t.v[0][2], t.v[0][3]);
		ImGui::Text(" %.3f  %.3f  %.3f  %.3f", t.v[1][0], t.v[1][1], t.v[1][2], t.v[1][3]);
		ImGui::Text(" %.3f  %.3f  %.3f  %.3f", t.v[2][0], t.v[2][1], t.v[2][2], t.v[2][3]);
		ImGui::Text(" %.3f  %.3f  %.3f  %.3f", t.v[3][0], t.v[3][1], t.v[3][2], t.v[3][3]);

		t = world_transform;
		ImGui::Text("World transform: ");
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

    local_transform = local_transform.FromTRS(pos, rot, s);

	CalculateWorldTransform();

	return local_transform;
}

void ComponentTransform::SetTransform(float4x4 &mat)
{
	local_transform = mat;
	local_transform.Decompose(position, rotation, scale);

	CalculateWorldTransform();
}

void ComponentTransform::SetPosition(float3 &p)
{
	position = p;

	local_transform.SetTranslatePart(p);

	CalculateWorldTransform();
}

void ComponentTransform::SetRotation(float3 &r)
{
	float3 rot = math::DegToRad(r);

	rotation = rotation.FromEulerXYZ(rot.x, rot.y, rot.z);

	local_transform = local_transform.FromTRS(position, rotation, scale);

	CalculateWorldTransform();
}
void ComponentTransform::SetScale(float3 &s)
{
	scale = s;

	//NOTE: Theres Scale() but not sure it will overlap the local_transform
	local_transform = local_transform.FromTRS(position, rotation, scale);

	CalculateWorldTransform();
}

float4x4 ComponentTransform::GetLocalTransform() const
{
	return local_transform;
}

float4x4 ComponentTransform::GetWorldTransform() const
{
	return world_transform;
}

float4x4 ComponentTransform::GetPreviousWorldTransform() const
{
	return previous_world_transform;
}

float4x4 ComponentTransform::GetFinalTransformMatrix() const
{
	return world_transform.Transposed();
}

void ComponentTransform::CalculateWorldTransform()
{

	//NOTE: do it more secure
	if (go->GetParent() && go->GetParent()->GetComponentTransform())
	{
		ComponentTransform* parent_transform = go->GetParent()->GetComponentTransform();

		//NOTE: if errors, check this
		world_transform = parent_transform->GetWorldTransform() * local_transform;
	}
	else
		world_transform = local_transform;

	if (!go->children.empty())
	{
		for (uint i = 0, size = go->children.size(); i < size; i++)
		{
			go->children[i]->GetComponentTransform()->CalculateWorldTransform();
		}
	}

	go->OnTransform();

	//Better here so i don't have to transfer much info
	go->SetOBB(world_transform, previous_world_transform);

	previous_world_transform = world_transform;
}