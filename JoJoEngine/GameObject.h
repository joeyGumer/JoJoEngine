#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <vector>
#include "Math.h"

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

	void SetAABB(float* vertices, int n_vertices);
	void SetOBB(float4x4 &trans, float4x4& previous_trans);

private:
	void UpdateComponents();

public:
	std::string name;
	std::vector<GameObject*> children;
	std::vector<Component*> components;

	OBB bb_object;
	AABB bb_axis;

	bool to_draw = false;

private:
	bool active = true;

	ComponentTransform* comp_transform = nullptr;
	GameObject* parent = nullptr;

	long unsigned int UUID;

};

#endif // !_GAMEOBJECT_H_

