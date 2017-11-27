#ifndef _COMPONENTTRANSFORM_H_
#define _COMPONENTTRANSFORM_H_

#include "Component.h"
#include "Math.h"

enum TRANS_OPERATION
{
	TRANSLATE,
	ROTATE,
	SCALE
};

enum TRANS_MODE
{
	LOCAL,
	WORLD,
};

class ComponentTransform : public Component
{
public:

	ComponentTransform(GameObject* go);
	~ComponentTransform();

	void Update();

	void OnEditor();

	void SetPosition(float3 &p);
	void SetRotation(float3 &r);
	void SetScale(float3 &s);

	float4x4 SetTransform(float3 &pos, Quat &rot, float3 &s);
	void SetTransform(float4x4 &mat);

	float4x4 GetLocalTransform() const;
	float4x4 GetWorldTransform() const;
	float4x4 GetPreviousWorldTransform() const;
	float4x4 GetFinalTransformMatrix() const;

	void CalculateWorldTransform();
	
private:

	float4x4 local_transform;
	float4x4 world_transform;
	float4x4 previous_world_transform;

	float3 position;
	Quat rotation;
	float3 scale;
public:

	TRANS_OPERATION operation = TRANSLATE;
	TRANS_MODE mode = WORLD;
};


#endif
