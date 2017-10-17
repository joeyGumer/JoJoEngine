#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"

struct Mesh;
class ComponentMesh : public Component
{
public:
	ComponentMesh();
	ComponentMesh(Mesh* m);

	~ComponentMesh();

	void Update();

	void OnEditor();

private:
	Mesh* mesh;
};

#endif