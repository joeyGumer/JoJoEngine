#include "ModuleFBXLoader.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneEdit.h"

#include "GameObject.h"
#include "Component.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "Globals.h"
#include "Math.h"


#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

//NOTE: temporal use of OpenGL


Mesh::~Mesh()
{
	RELEASE(vertices);
	RELEASE(indices);
	RELEASE(normals);
	RELEASE(texture_UVs);
	RELEASE(colors);
}


ModuleFBXLoader::ModuleFBXLoader(bool start_enabled): Module(start_enabled)
{

}

ModuleFBXLoader::~ModuleFBXLoader()
{

}

bool ModuleFBXLoader::Init()
{
	bool ret = true;

	//Init devil 
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleFBXLoader::CleanUp()
{
	bool ret = true;

	//Devil cleanup
	ilShutDown();

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

Mesh** ModuleFBXLoader::LoadFBX(const char* file_path) const
{
	//bool ret = true;

	LOG("Loading new FBX into scene--------------------");

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	Mesh** ret = nullptr;

	if (scene != nullptr && scene->HasMeshes())
		{

			uint num_meshes = scene->mNumMeshes;
			ret = new  Mesh*[num_meshes];

			//NOTE: see for a way to count all the meshes
			/*if (n_mesh)
				*n_mesh = num_meshes;*/

			//Loading all nodes
			aiNode* root = scene->mRootNode;

			LoadNode(scene, root, App->level->root_GO);

			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			//NOTE: for now still use this
			for (uint i = 0; i < scene->mNumMeshes; i++)
			{
				ret[i] = LoadMesh(scene->mMeshes[i]);
			}

			//Get texture
			//NOTE: for now only the renderer needs it
			if (scene->HasMaterials())
			{
				//NOTE: for now, we just want one texture
				aiString texture_path;

				scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);

				//NOTE: call this here or make the function return the path?
				App->renderer3D->LoadImageTexture(texture_path.C_Str());
			}


			//Get transform
			//NOTE: temporal, until we use GO, we'll show the scene transform

			aiMatrix4x4 root_transform = scene->mRootNode->mTransformation;
			float4x4 tr;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					tr.v[i][j] = root_transform[i][j];
				}
			}
			
			App->renderer3D->transform = tr;

			//Release the scene
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
Mesh* ModuleFBXLoader::LoadMesh(const aiMesh* new_mesh) const 
{
	LOG("Loading new Mesh into scene from the FBX---------------");

	Mesh* m = new Mesh();

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

		LOG("New mesh with %d faces", new_mesh->mNumFaces);
	}

	//normals
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

		for (int i = 0; i < m->num_texture_UVs; i ++)
		{
			memcpy(&m->texture_UVs[i * 2], &new_mesh->mTextureCoords[0][i].x, sizeof(float));
			memcpy(&m->texture_UVs[(i * 2) + 1], &new_mesh->mTextureCoords[0][i].y, sizeof(float));

			//m->texture_UVs[i * 2] = new_mesh->mTextureCoords[0][i].x;
			//m->texture_UVs[(i * 2) + 1] = new_mesh->mTextureCoords[0][i].y;
		}
		LOG("New mesh with %d UVs", m->num_texture_UVs);
	}


	return m;

}

void ModuleFBXLoader::LoadNode(const aiScene* scene, aiNode* new_node, GameObject* go ) const
{	
	GameObject* GO = App->level->AddGameObject(new_node->mName.C_Str(), go);

	//Component Transform
	ComponentTransform* comp_transform = (ComponentTransform*)GO->AddComponent(COMP_TRANSFORM);

	aiMatrix4x4 transform = new_node->mTransformation;
	
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;

	new_node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	comp_transform->SetTransform(pos, rot, scale);
	//----------------------


	//Component Mesh	
	for (uint i = 0, size = new_node->mNumMeshes; i < size; i++)
	{
		ComponentMesh* comp_mesh = (ComponentMesh*)GO->AddComponent(COMP_MESH);

		aiMesh* m = scene->mMeshes[new_node->mMeshes[i]];
		Mesh* new_mesh = LoadMesh(m);
		comp_mesh->SetMesh(new_mesh);

		//Component Material (from mesh)
		//NOTE: if different meshes have same material is necessary to load the same texture again?
		ComponentMaterial* comp_material = (ComponentMaterial*)GO->AddComponent(COMP_MATERIAL);

		aiMaterial* material = scene->mMaterials[m->mMaterialIndex];

		//NOTE: take number of textures in acount
		uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		uint texture;
		float2 tex_size;

		LoadTexture(path.C_Str(), &texture, &tex_size.x, &tex_size.y);

		comp_material->SetTexture(texture, tex_size.x, tex_size.y);
	}

	//---------------------

	//Iteratin all children
	for (uint i = 0, num_children = new_node->mNumChildren; i < num_children; i++)
	{
		LoadNode(scene, new_node->mChildren[i], GO);
	}	
}

bool  ModuleFBXLoader::LoadTexture(const char* file, uint* texture, float* size_x, float* size_y)const
{
	bool ret = true;

	ILuint id_image;
	ilGenImages(1, &id_image);
	ilBindImage(id_image);

	ret = ilLoadImage(file);

	if (ret)
	{
		//NOTE: maybe check if the image is a power of two?

		*texture = ilutGLBindTexImage();

		if (size_x && size_y)
		{
			*size_x = ilGetInteger(IL_IMAGE_WIDTH);
			*size_y = ilGetInteger(IL_IMAGE_HEIGHT);
		}
	}
	else
	{
		LOG("Error: failure trying to load texture %s", file);
	}

	ilDeleteImages(1, &id_image);

	return ret;
}