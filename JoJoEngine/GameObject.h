#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <vector>

class Component;
class ComponentTransform;
enum TypeComp;

class GameObject
{
public:
	GameObject();
	GameObject(const char* n, GameObject* p = nullptr);
	~GameObject();

	void Update();
	Component* AddComponent(TypeComp type);

	//Slow (don't know if will be needed)
	bool ReleaseComponent(Component* comp);

	GameObject* GetParent() const;
	ComponentTransform* GetComponentTransform() const;
	Component* GetComponent(TypeComp type)const;

private:
	void UpdateComponents();

public:
	std::string name;
	std::vector<GameObject*> children;
	std::vector<Component*> components;

private:
	bool active = true;

	ComponentTransform* comp_transform = nullptr;
	GameObject* parent = nullptr;

};

#endif // !_GAMEOBJECT_H_

