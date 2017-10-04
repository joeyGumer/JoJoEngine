#include "ModuleFBXLoader.h"
#include "Application.h"
#include "Globals.h"
#include "Math.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleFBXLoader::ModuleFBXLoader(bool start_enabled): Module(start_enabled)
{

}

ModuleFBXLoader::~ModuleFBXLoader()
{

}

bool ModuleFBXLoader::Init()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleFBXLoader::CleanUp()
{
	bool ret = true;

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

std::vector<Model3D> ModuleFBXLoader::LoadFBX(char* file_path)
{
	std::vector<Model3D> ret;

	//bool ret = true;

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			ret.push_back(LoadMesh(scene->mMeshes[i]));
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", file_path);
		//ret = false;
	}
	
	return ret;
}

//NOTE: using pointers?
Model3D ModuleFBXLoader::LoadMesh(aiMesh* new_mesh)
{
	Model3D m;

	//Copy vertices
	m.num_vertices = new_mesh->mNumVertices;
	m.vertices = new float[m.num_vertices * 3];
	memcpy(m.vertices, new_mesh->mVertices, sizeof(float) * m.num_vertices * 3);
	LOG("New mesh with %d vertices", m.num_vertices);

	// copy faces
	if (new_mesh->HasFaces())
	{
		m.num_indices = new_mesh->mNumFaces * 3;
		m.indices = new uint[m.num_indices]; // assume each face is a triangle
		for (uint i = 0; i < new_mesh->mNumFaces; i++)
		{
			if (new_mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else//NOTE: is memcopy really necesary instead of a for?
				memcpy(&m.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	return m;
}