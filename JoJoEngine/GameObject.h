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
	void Draw();
	Component* AddComponent(TypeComp type);

	//Slow (don't know if will be needed)
	bool ReleaseComponent(Component* comp);

	GameObject* GetParent() const;
	ComponentTransform* GetComponentTransform() const;
	float4x4 GetTransform()const;
	Component* GetComponent(TypeComp type)const;
	bool GetCastRayDistance(LineSegment& ray, float* distance, float3* hit_point) const;

	bool IsStatic()const;
	bool HasMesh()const;


	void SetAABB(float* vertices, int n_vertices);
	void SetOBB(float4x4 &trans, float4x4& previous_trans);
	void SetStatic(bool is_stat);

	void OnTransform();

private:
	void UpdateComponents();

public:
	std::string name;
	std::vector<GameObject*> children;
	std::vector<Component*> components;

	OBB bb_object;
	AABB bb_axis;

	bool to_draw = true;

private:
	bool active = true;
	bool is_static = true;

	ComponentTransform* comp_transform = nullptr;
	GameObject* parent = nullptr;

	long unsigned int UUID;

};

#endif // !_GAMEOBJECT_H_

