#ifndef  _IMPORTERMESH_H_
#define _IMPORTERMESH_H_

#include "Importer.h"

#include <string>

struct Mesh;
struct aiMesh;

//NOTE: namespace or better a class?
namespace ImporterMesh
{

	bool Import(aiMesh* ai_mesh, std::string& output_filename);
	bool LoadAssimpMesh(Mesh* mesh, aiMesh* ai_mesh);
	bool SaveAssimpMesh(Mesh& mesh, std::string& output_filename);

	bool Load();
};

#endif
