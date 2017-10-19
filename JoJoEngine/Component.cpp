#include "Component.h"
#include "GameObject.h"

Component::Component(TypeComp t, GameObject* g) : type(t), go(g)
{

}

void Component::SetGameObject(GameObject* g)
{
	go = g;
	//go->AddComponent(this);
}

void Component::ReleaseGameObject()
{
	//go->
}

void Component::SetActive(bool act)
{
	if (active != act)
	{
		active = act;
		if (active)
		{
			Enable();
		}
		else
			Disable();
	}
}

bool Component::IsActive() const
{
	return active;
}