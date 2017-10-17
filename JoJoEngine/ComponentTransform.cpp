#include "ComponentTransform.h"

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

}