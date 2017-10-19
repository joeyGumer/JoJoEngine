#ifndef _COMPONENT_H_
#define _COMPONENT_H_


enum TypeComp
{
	COMP_TRANSFORM,
	COMP_MESH,
	COMP_MATERIAL,
};

class GameObject;
class Component
{
public:
	Component(TypeComp t, GameObject* g);
	virtual ~Component() {};

	virtual void Enable() {}
	virtual void Update() {}
	virtual void Disable(){}

	virtual void OnEditor() {}

	//NOTE: needed?
	void SetGameObject(GameObject* g);
	void ReleaseGameObject();

private:
	TypeComp type;
	GameObject* go = nullptr;
};
#endif
