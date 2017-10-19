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