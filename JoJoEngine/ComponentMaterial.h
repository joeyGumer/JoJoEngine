#ifndef _COMPONENTMATERIAL_H_
#define _COMPONENTMATERIAL_H_

#include "Component.h"
#include "Math.h"
#include "Globals.h"


struct Mesh;
class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* g);

	~ComponentMaterial();

	void Update();

	void OnEditor();

	void SetTexture(uint texture, float x, float y);

private:
	uint texture;
	float2 size;
};

#endif
