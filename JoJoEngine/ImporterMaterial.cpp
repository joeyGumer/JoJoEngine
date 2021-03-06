#include "ImporterMaterial.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

bool ImporterMaterial::Import(const char* file_name, std::string& output)
{
	bool ret = true;

	char* buffer = nullptr;

	std::string name;
	App->fs->GetFileName(file_name, name).c_str();
	uint buffer_size = App->fs->Load(name.c_str(), &buffer);

	ILuint id_image;
	ilGenImages(1, &id_image);
	ilBindImage(id_image);

	ret = ilLoadL(IL_TYPE_UNKNOWN, buffer, buffer_size);

	if (ret)
	{
		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
		if (size > 0) {
			data = new ILubyte[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			{
				//NOTE: should do it in a more secure way?
				ret = App->fs->SaveUnique("Material", (char*)data, size, LIBRARY_MATERIALS, "dds", output);
			}
			RELEASE_ARRAY(data);
		}
	}

	ilDeleteImages(1, &id_image);

	return ret;
}

bool ImporterMaterial::Load(const char* file_name, int * texture)
{
	bool ret = false;

	//NOTE: not sure if i have to use ilLoadL too, but the only way to find the file is through FileSystem
	char* buffer = nullptr;
	uint buffer_size = App->fs->Load(file_name, &buffer);

	ILuint id_image;
	ilGenImages(1, &id_image);
	ilBindImage(id_image);

	ret = ilLoadL(IL_TYPE_UNKNOWN, buffer, buffer_size);
	

	if (ret)
	{
		*texture = ilutGLBindTexImage();
	}
	else
	{
		LOG("Error: failure trying to load texture %s: %s", file_name, iluErrorString(ilGetError()));
	}

	ilDeleteImages(1, &id_image);

	return ret;
}

