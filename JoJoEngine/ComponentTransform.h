#ifndef _COMPONENTTRANSFORM_H_
#define _COMPONENTTRANSFORM_H_

#include "Component.h"
#include "Math.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	ComponentTransform(float4x4 &mat);
	ComponentTransform(float3 &p, Quat &r, float3 &s);
	~ComponentTransform();

	void Update();

	void OnEditor();

private:

	float4x4 local_transform;
	float3 position;
	Quat rotation;
	float3 scale;	
};


#endif
