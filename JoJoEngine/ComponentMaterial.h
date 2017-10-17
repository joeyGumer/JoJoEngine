#ifndef _COMPONENTMATERIAL_H_
#define _COMPONENTMATERIAL_H_

#include "Component.h"


struct Mesh;
class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(Mesh* m);

	~ComponentMaterial();

	void Update();

	void OnEditor();

private:
	int texture;
};

#endif
