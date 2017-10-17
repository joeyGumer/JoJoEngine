#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <vector>

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();

private:
	void UpdateComponents();

private:

	std::string name;
	bool active;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;
};

#endif // !_GAMEOBJECT_H_

