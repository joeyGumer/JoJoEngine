#include "GameObject.h"
#include "Globals.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}


void GameObject::Update()
{
	UpdateComponents();

	for (uint i = 0, size = components.size; i < size; i++)
	{
		children[i]->Update();
	}
}

void GameObject::UpdateComponents()
{

	for (uint i = 0, size = components.size; i < size; i++)
	{
		//components[i]->Update();
	}
}