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

Model3D** ModuleFBXLoader::LoadFBX(char* file_path, uint& n_mesh)
{
	//bool ret = true;

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	Model3D** ret = nullptr;

	if (scene != nullptr && scene->HasMeshes())
		{

			uint num_meshes = scene->mNumMeshes;
			ret = new  Model3D*[num_meshes];

			if (n_mesh)
				n_mesh = num_meshes;

			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			for (uint i = 0; i < scene->mNumMeshes; i++)
			{
				ret[i] = LoadMesh(scene->mMeshes[i]);
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
Model3D* ModuleFBXLoader::LoadMesh(aiMesh* new_mesh)
{
	//NOTE: look for where to delete all the new Model3D created

	Model3D* m = new Model3D();

	//Copy vertices
	m->num_vertices = new_mesh->mNumVertices;
	m->vertices = new float[m->num_vertices * 3];
	memcpy(m->vertices, new_mesh->mVertices, sizeof(float) * m->num_vertices * 3);
	LOG("New mesh with %d vertices", m->num_vertices);

	//Copy faces/indices
	if (new_mesh->HasFaces())
	{
		m->num_indices = new_mesh->mNumFaces * 3;
		m->indices = new uint[m->num_indices]; // assume each face is a triangle
		for (uint i = 0; i < new_mesh->mNumFaces; i++)
		{
			if (new_mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else//NOTE: is memcopy really necesary instead of a for?
				memcpy(&m->indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	if (new_mesh->HasNormals())
	{
		m->num_normals = new_mesh->mNumVertices;
		m->normals = new float[m->num_normals * 3];

		memcpy(m->normals, new_mesh->mNormals, sizeof(float) * m->num_vertices * 3);
		LOG("New mesh with %d normals", m->num_vertices);
	}

	//Copy textures (only one set of UVs)
	if (new_mesh->HasTextureCoords(0))
	{
		m->num_texture_UVs = new_mesh->mNumVertices;
		m->texture_UVs = new float[m->num_texture_UVs * 2];

		//NOTE: There may be an error here
		memcpy(m->texture_UVs, new_mesh->mTextureCoords, sizeof(float) * m->num_vertices * 2);
		LOG("New mesh with %d UVs", m->num_texture_UVs);
	}

	//Copy colors
	return m;
}