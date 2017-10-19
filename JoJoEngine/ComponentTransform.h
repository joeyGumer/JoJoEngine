#ifndef _COMPONENTTRANSFORM_H_
#define _COMPONENTTRANSFORM_H_

#include "Component.h"
#include "Math.h"

class ComponentTransform : public Component
{
public:

	ComponentTransform(GameObject* go);
	~ComponentTransform();

	void Update();

	void OnEditor();

	float4x4 SetTransform(float3 &pos, Quat &rot, float3 &s);
	void SetTransform(float4x4 &mat);

private:

	float4x4 local_transform;
	/*float3 position;
	Quat rotation;
	float3 scale;*/	
};


#endif
