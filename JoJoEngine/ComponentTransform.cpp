#include "ComponentTransform.h"

ComponentTransform::ComponentTransform() : Component(COMP_TRANSFORM)
{
	transform.SetIdentity();
}
ComponentTransform::ComponentTransform(float3 &p, Quat &r, float3 &s ) : Component(COMP_TRANSFORM), position(p), rotation(r), scale(s)
{
	transform = transform.FromTRS(position, rotation, scale);
}

ComponentTransform::~ComponentTransform()
{

}

void ComponentTransform::Update()
{

}