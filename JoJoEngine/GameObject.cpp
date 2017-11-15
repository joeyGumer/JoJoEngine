#include "GameObject.h"
#include "Component.h"
#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "MathGeoLib\Include\Algorithm\Random\LCG.h"

GameObject::GameObject()
{
	math::LCG random;
	UUID = random.Int();

	bb_axis.SetNegativeInfinity();
	bb_object.SetNegativeInfinity();
}

GameObject::GameObject(const char* n, GameObject* p): name(n), parent(p)
{
	math::LCG random;
	UUID = random.Int();

	if (parent)
	{
		parent->children.push_back(this);
	}

	bb_axis.SetNegativeInfinity();
	bb_object.SetNegativeInfinity();
}
GameObject::~GameObject()
{
	for (uint i = 0, size = children.size(); i < size; i++)
	{
		RELEASE(children[i]);
	}

	children.clear();
}


void GameObject::Update()
{
	UpdateComponents();

	for (uint i = 0, size = children.size(); i < size; i++)
	{
		children[i]->Update();
	}
}

void GameObject::UpdateComponents()
{
	for (uint i = 0, size = components.size(); i < size; i++)
	{
		if (components[i]->IsActive())
		{
			components[i]->Update();
		}
	}
}

Component* GameObject::AddComponent(TypeComp type)
{	
	Component* new_comp = nullptr;

	//NOTE: Check if we have certain components already
	switch (type)
	{
	case COMP_TRANSFORM:
		new_comp = new ComponentTransform(this);
		comp_transform = (ComponentTransform*)new_comp;
		break;
	case COMP_MESH:
		new_comp = new ComponentMesh(this);
		break;
	case COMP_MATERIAL:
		new_comp = new ComponentMaterial(this);
		break;
	case COMP_CAMERA:
		new_comp = new ComponentCamera(this);
		break;
	default:
		break;
	}

	if (new_comp)
	{
		components.push_back(new_comp);
	}

	return new_comp;
}

//Slow but, as we need efficiency in-game and components won't be released then, we use a vector either way
bool GameObject::ReleaseComponent(Component* comp)
{
	if (comp)
	{
		for (uint i = 0, size = components.size(); i < size; i++)
		{
			if (components[i] == comp)
			{
				components.erase(components.begin() + i);
				return true;
			}
		}
	}

	return false;
}

void GameObject::OnTransform()
{
	if (is_static)
		is_static = false;

	for (uint i = 0, size = components.size(); i < size; i++)
	{
		components[i]->OnTransform();
	}

}

GameObject* GameObject::GetParent() const
{
	return parent;
}

ComponentTransform* GameObject::GetComponentTransform() const
{
	return comp_transform;
}

float4x4 GameObject::GetTransform() const
{
	return ((ComponentTransform*)GetComponent(COMP_TRANSFORM))->GetWorldTransform();
}

Component* GameObject::GetComponent(TypeComp type) const
{
	if(type != COMP_TRANSFORM)
	{
		for (uint i = 0, size = components.size(); i < size; i++)
		{
			//NOTE: make a check because it may have more than one comp of the same type
			if (components[i]->GetComponentType() == type)
			{
				return components[i];
			}
		}
		//NOTE: LOG here
		return nullptr;
	}
	else
		return comp_transform;
}

void GameObject::SetAABB(float* vertices, int n_vertices)
{
	bb_axis.SetNegativeInfinity();
	bb_object.SetNegativeInfinity();

	//bb_axis.Enclose((float3 *)vertices, n_vertices);


	//NOTE: very slow alternative, should watch for an alternative
	bb_object = bb_object.OptimalEnclosingOBB((float3*)vertices, n_vertices);
	//bb_axis = bb_object.MinimalEnclosingAABB();

	float4x4 trans = ((ComponentTransform*)GetComponent(COMP_TRANSFORM))->GetWorldTransform();
	float4x4 prev = float4x4::identity;
	SetOBB(trans, prev);
}

void GameObject::SetOBB(float4x4& trans, float4x4& previous_trans)
{
	//NOTE: look if i can do this without using the inversed

	bb_object.Transform(previous_trans.Inverted() * trans);

	if (bb_object.IsDegenerate())
	{
		//Mesh* m = ((ComponentMesh*)GetComponent(COMP_MESH)).;
	}

	bb_axis = bb_object.MinimalEnclosingAABB();
}