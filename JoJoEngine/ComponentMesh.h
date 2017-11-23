#ifndef _COMPONENTMESH_H_
#define _COMPONENTMESH_H_

#include "Component.h"
#include "Math.h"
#include "Globals.h"

struct Mesh
{
	~Mesh();

	//Vertices
	uint id_vertices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	//Indices
	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	//Normals
	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	//Textures
	uint id_texture_UVs = 0;
	uint num_texture_UVs = 0;
	float* texture_UVs = nullptr;

	//Colors
	uint id_colors = 0;
	uint num_colors = 0;
	uint* colors = nullptr;
};

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* g);

	~ComponentMesh();

	void Update();
	void Draw();

	void OnEditor();

	void SetMesh(Mesh* m);
	
	int GetTriangles() const;
	Mesh* GetMesh() const;
	void GetTrianglesList(std::vector<Triangle>& tris) const;

private:
	Mesh* mesh;


};

#endif