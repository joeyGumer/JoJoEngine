#include "GameObject.h"
#include "Component.h"
#include "Globals.h"

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
		components[i]->Update();
	}
}