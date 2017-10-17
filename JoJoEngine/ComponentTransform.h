#ifndef _COMPONENTTRANSFORM_H_
#define _COMPONENTTRANSFORM_H_

#include "Component.h"
#include "Math.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	ComponentTransform(float3 &p, Quat &r, float3 &s);
	~ComponentTransform();

	void Update();

private:

	float4x4 transform;
	float3 position;
	Quat rotation;
	float3 scale;	
};


#endif
