#ifndef _MODULEFBXLOADER_H_
#define _MODULEFBXLOADER_H_

#include "Module.h"
#include <vector>

struct aiMesh;

struct Model3D
{
	~Model3D();

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
	

};

class ModuleFBXLoader : public Module
{
public:
	ModuleFBXLoader(bool start_enabled = true);
	virtual ~ModuleFBXLoader();

	bool Init();
	bool CleanUp();

	//NOTE: use vector or array?
	Model3D** LoadFBX(char* file_path, uint* n_mesh);
private:
	Model3D* LoadMesh(aiMesh* new_mesh);

};
#endif // !_MODULEFBXLOADER_H_
