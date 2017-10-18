#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <vector>

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const char* n, GameObject* p = nullptr);
	~GameObject();

	void Update();
	bool AddComponent(Component* comp);

private:
	void UpdateComponents();

public:
	std::string name;
	std::vector<GameObject*> children;
	std::vector<Component*> components;

private:
	bool active = true;


	GameObject* parent = nullptr;

};

#endif // !_GAMEOBJECT_H_

