#ifndef _MODULEFBXLOADER_H_
#define _MODULEFBXLOADER_H_

#include "Module.h"
#include <vector>

class GameObject;
struct aiMesh;
struct aiNode;
struct aiScene;
struct Mesh;

class ModuleFBXLoader : public Module
{
public:
	ModuleFBXLoader(bool start_enabled = true);
	virtual ~ModuleFBXLoader();

	bool Init();
	bool CleanUp();

	Mesh** LoadFBX(const char* file_path) const;

private:
	Mesh* LoadMesh(const aiMesh* new_mesh)const;
	bool  LoadTexture(const char* file, uint* texture, float* size_x = nullptr, float* size_y = nullptr)const;
	void LoadNode(const aiScene* scene, aiNode* new_node, GameObject* go)const;
	

};
#endif // !_MODULEFBXLOADER_H_
