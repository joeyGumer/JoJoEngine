#include "ModuleFBXLoader.h"

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