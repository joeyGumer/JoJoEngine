#ifndef _COMPONENTMATERIAL_H_
#define _COMPONENTMATERIAL_H_

#include "Component.h"
#include "Math.h"
#include "Globals.h"


struct Mesh;
class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(uint tex, float2 &s);

	~ComponentMaterial();

	void Update();

	void OnEditor();

private:
	uint texture;
	float2 size;
};

#endif
