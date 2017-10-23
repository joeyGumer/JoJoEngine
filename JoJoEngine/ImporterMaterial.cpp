#include "ImporterMaterial.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

bool Importer::ImporterMaterial::Import(const char* file_name)
{
	bool ret = true;

	char* buffer = nullptr;
	uint buffer_size = App->fs->Load(file_name, &buffer);

	

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
				ret = App->fs->Save(file_name, (char*)data, size);
			}
			RELEASE_ARRAY(data);
		}
	}

	return ret;
}

bool Importer::ImporterMaterial::Load(const char* file_name, int * texture)
{
	bool ret = true;


	return ret;
}

