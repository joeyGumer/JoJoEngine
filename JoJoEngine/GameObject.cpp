#include "GameObject.h"
#include "Component.h"
#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject()
{
	
}

GameObject::GameObject(const char* n, GameObject* p): name(n), parent(p)
{
	if (parent)
	{
		parent->children.push_back(this);
	}
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

GameObject* GameObject::GetParent() const
{
	return parent;
}

ComponentTransform* GameObject::GetComponentTransform() const
{
	return comp_transform;
}