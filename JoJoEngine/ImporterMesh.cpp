#include "ImporterMesh.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

//NOTE: should not be included here, temporal measure
#include "ModuleFBXLoader.h"

bool ImporterMesh::Import(aiMesh* ai_mesh ,std::string& output_filename)
{
	Mesh mesh;

	LoadAssimpMesh(&mesh, ai_mesh);
	SaveAssimpMesh(mesh, output_filename);

	return true;
}

Mesh* ImporterMesh::Load(const char* filename)
{
	Mesh* mesh = new Mesh();

	char* buffer;
	App->fs->Load(filename, &buffer);

	char* cursor = buffer;

	// amount of indices / vertices / normals / texture_coords / colors 
	uint ranges[5];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	//Data groups quantity
	mesh->num_indices = ranges[0];
	mesh->num_vertices = ranges[1];
	mesh->num_normals = ranges[2];
	mesh->num_texture_UVs = ranges[3];
	mesh->num_colors = ranges[4];

	//Data groups
	uint vert_size = mesh->num_vertices * 3 * sizeof(float);
	uint ind_size = mesh->num_indices * sizeof(uint);
	uint norm_size = mesh->num_normals * 3 * sizeof(float);
	uint uv_size = mesh->num_texture_UVs * 2 * sizeof(float);
	uint color_size = mesh->num_colors * 3 * sizeof(uint);

	//Indices
	cursor += bytes;
	bytes = ind_size;
	mesh->indices = new uint[ind_size];
	memcpy(mesh->indices, cursor, bytes);

	//Vertices
	cursor += bytes;
	bytes = vert_size;
	mesh->vertices = new float[vert_size];
	memcpy(mesh->vertices, cursor, bytes);

	//Normals
	cursor += bytes;
	bytes = norm_size;
	mesh->normals = new float[norm_size];
	memcpy(mesh->normals, cursor, bytes);

	//UVs
	cursor += bytes;
	bytes = uv_size;
	mesh->texture_UVs = new float[uv_size];
	memcpy(mesh->texture_UVs, cursor, bytes);

	//Colors
	cursor += bytes;
	bytes = color_size;
	mesh->colors = new uint[color_size];
	memcpy(mesh->colors, cursor, bytes);

	return mesh;
}

bool ImporterMesh::LoadAssimpMesh(Mesh* m, aiMesh* new_mesh)
{
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

		for (int i = 0; i < m->num_texture_UVs; i++)
		{
			memcpy(&m->texture_UVs[i * 2], &new_mesh->mTextureCoords[0][i].x, sizeof(float));
			memcpy(&m->texture_UVs[(i * 2) + 1], &new_mesh->mTextureCoords[0][i].y, sizeof(float));

			//m->texture_UVs[i * 2] = new_mesh->mTextureCoords[0][i].x;
			//m->texture_UVs[(i * 2) + 1] = new_mesh->mTextureCoords[0][i].y;
		}
		LOG("New mesh with %d UVs", m->num_texture_UVs);
	}

	//Colors
	return true;
}

bool ImporterMesh::SaveAssimpMesh(Mesh& mesh, std::string& output_filename)
{
	bool ret = false;

	//Ranges info as header for the file
	uint ranges[5] = { mesh.num_indices, mesh.num_vertices,  mesh.num_normals, mesh.num_texture_UVs , mesh.num_colors };

	//All data groups size
	uint vert_size = mesh.num_vertices * 3 * sizeof(float);
	uint ind_size = mesh.num_indices * sizeof(uint);
	uint norm_size = mesh.num_normals * 3 * sizeof(float);
	uint uv_size = mesh.num_texture_UVs * 2 * sizeof(float);
	uint color_size = mesh.num_colors * 3 * sizeof(uint);

	//Total size of the buffer
	uint size = vert_size + ind_size + norm_size + uv_size + sizeof(ranges);

	char* data = new char[size];
	char* cursor = data;

	//Ranges info as header
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	//Indices
	cursor += bytes;
	bytes = ind_size;
	memcpy(cursor, mesh.indices, bytes);

	//Vertices
	cursor += bytes;
	bytes = vert_size;
	memcpy(cursor, mesh.vertices, bytes);

	//Normals
	cursor += bytes;
	bytes = norm_size;
	memcpy(cursor, mesh.normals, bytes);

	//UVs
	cursor += bytes;
	bytes = uv_size;
	memcpy(cursor, mesh.texture_UVs, bytes);

	//Colors
	cursor += bytes;
	bytes = color_size;
	memcpy(cursor, mesh.colors, bytes);

	ret = App->fs->SaveUnique("Mesh", data, size, "Library/", "jme", output_filename);

	RELEASE_ARRAY(data);

	return ret;
}