#ifndef _MODULEFBXLOADER_H_
#define _MODULEFBXLOADER_H_

#include "Module.h"
#include <vector>

struct aiMesh;

struct Model3D
{
	//Vertices
	uint id_vertices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	//Indices
	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
};

class ModuleFBXLoader : public Module
{
public:
	ModuleFBXLoader(bool start_enabled = true);
	virtual ~ModuleFBXLoader();

	bool Init();
	bool CleanUp();

	//NOTE: use vector or array?
	std::vector<Model3D> LoadFBX(char* file_path);
private:
	Model3D LoadMesh(aiMesh* new_mesh);

};
#endif // !_MODULEFBXLOADER_H_
