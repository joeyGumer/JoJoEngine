#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"
#include "Math.h"

struct Mesh;
class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* g);

	~ComponentMesh();

	void Update();

	void OnEditor();

	void SetMesh(Mesh* m);
	
	int GetTriangles() const;
	void GetTrianglesList(std::vector<Triangle>& tris) const;

private:
	Mesh* mesh;


};

#endif