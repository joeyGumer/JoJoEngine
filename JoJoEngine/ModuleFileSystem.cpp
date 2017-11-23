#include "ModuleFileSystem.h"
#include "Application.h"

#include <algorithm>

#include "JSON/parson.h"
#include "SDL/include/SDL.h"
#include "PhysFS\include\physfs.h"


#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

ModuleFileSystem::ModuleFileSystem() : Module()
{
	name = "file_system";

	// need to be created before Awake so other modules can use it
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	LOG("FileSystem Operations base is [%s] plus:", GetBasePath());

	// By default we include executable's own directory
	// without this we won't be able to find config.json :-(
	AddPath(".");
	AddPath(GetBasePath());

}

// Destructor
ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
}

// Called before render is available
bool ModuleFileSystem::LoadConfig(JSON_Object* data)
{
	// Ask SDL for a write dir
	const char* write_path = GetBasePath();
	//(char*)PHYSFS_getBaseDir();
	
	if (PHYSFS_setWriteDir(write_path) == 0)
	{
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}
	else
	{
		// We add the writing directory as a reading directory too with speacial mount point
		LOG("Writing directory is %s\n", write_path);
		//AddPath(write_path, GetSaveDirectory());
	}


	//NOTE: may give an error
	//SDL_free(write_path);


	//NOTE: create Assets and Library folders if they don't exist, not sure if do it here
	if (!Exists("Assets"))
	{
		CreateDirectoryFile("Assets");
	}
	if (!Exists("Library"))
	{
		CreateDirectoryFile("Library");
	}
	if (!Exists(LIBRARY_MESHES))
	{
		CreateDirectoryFile(LIBRARY_MESHES);
	}
	if (!Exists(LIBRARY_MATERIALS))
	{
		CreateDirectoryFile(LIBRARY_MATERIALS);
	}
	LOG("Loading File System");
	bool ret = true;

	// Add all paths in configuration in order
	JSON_Array* paths = json_object_get_array(data, "paths");

	for (uint i = 0, size = json_array_get_count(paths); i < size; i++)
	{
		char path[300];
		sprintf_s(path, 300, "%s%s", GetBasePath(), json_array_get_string(paths, i));
		AddPath(path);
	}

	AddPath(LIBRARY_MESHES);
	AddPath(LIBRARY_MATERIALS);

	//NOTE: testing
	PHYSFS_permitSymbolicLinks(1);





	return ret;
}

bool ModuleFileSystem::SaveConfig(JSON_Object* data) const
{
	bool ret = true;

	JSON_Value* paths = json_parse_string("a");
	json_object_set_value(data, "Paths", paths);

	json_value_free(paths);

	return ret;

}

// Called before quitting
bool ModuleFileSystem::CleanUp()
{
	//LOG("Freeing File System subsystem");
	return true;
}

// Add a new zip file or folder
bool ModuleFileSystem::AddPath(const char* path_or_zip, const char* mount_point)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, mount_point, 1) == 0)
	{
		LOG("File System error while adding a path or zip(%s): %s\n", path_or_zip, PHYSFS_getLastError());
	}
	else
	{
		LOG("Added new path to the File System: %s\n", path_or_zip);
	}
	ret = true;

	return ret;
}

// Check if a file exists
bool ModuleFileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

// Check if a file is a directory
bool ModuleFileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

bool ModuleFileSystem::CreateDirectoryFile(const char* directory)
{
	
	if (!PHYSFS_mkdir(directory))
	{
		LOG("Error trying to create directory: %s", PHYSFS_getLastError());
		return false;
	}

	LOG("New directory created: %s", directory);
	return true;
}

// Read a whole file and put it in a new buffer
unsigned int ModuleFileSystem::Load(const char* file, char** buffer) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fs_file, *buffer, 1, (PHYSFS_sint32)size);
			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
				ret = (uint)readed;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Read a whole file and put it in a new buffer
SDL_RWops* ModuleFileSystem::Load(const char* file) const
{
	char* buffer;
	int size = Load(file, &buffer);

	if (size > 0)
	{
		//NOTE: should i release the created buffer?
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != NULL)
			r->close = close_sdl_rwops;

		return r;
	}
	else
		return NULL;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
unsigned int ModuleFileSystem::Save(const char* file, const char* buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);

	if (fs_file != NULL)
	{
		PHYSFS_sint64 written = PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
			ret = (uint)written;

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::SaveUnique(const char* file, const char* buffer, unsigned int size, const char* path, const char* extension, std::string& output_filename)
{
	
	char file_name[100];
	uint name_size = 100;

	
	sprintf_s(file_name, name_size, "%s.%s", file, extension);

	std::vector<std::string> path_files;
	GetEnumerateFiles(path, path_files);

	bool unique = false;
	uint copies = 0;

	while (!unique)
	{
		unique = true;

		for (uint i = 0, size = path_files.size(); i < size; i++)
		{
			if (path_files[i].compare(file_name) == 0)
			{
				copies++;
				unique = false;
				
				sprintf_s(file_name, name_size, "%s%d.%s", file, copies, extension);
				break;
			}
		}		
	}

	output_filename = file_name;

	sprintf_s(file_name, name_size, "%s%s", path, output_filename.c_str());

	if (Save(file_name,buffer, size) > 0)
	{
		output_filename = file_name;
		return true;
	}

	return false;
}

//NOTE: issues returning char*
std::string ModuleFileSystem::GetFileExtension(const char* file) const
{
	std::string tmp_str = file;
	std::string ext;

	tmp_str.back();
	uint i = tmp_str.length() - 1;

	for (; tmp_str[i] != '.'; i--)
	{
	}

	for (; i < tmp_str.length(); i++)
	{
		ext.push_back(tmp_str[i]);
	}

	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	char* ret = (char*)ext.c_str();

	return ret;
}


std::string ModuleFileSystem::GetFileName(const char* path, std::string& output) const
{
	std::string file_name;
	std::string tmp_str = path;


	if (tmp_str.size() > 0)
	{
		std::string::iterator it = tmp_str.end() - 1;

		for (; it != tmp_str.begin() && (*it != '/') && (*it != '\\'); it--)
		{
		}

		it++;

		for (; it != tmp_str.end(); it++)
		{
			output.push_back(*it);
		}
	}

	char* ret = (char*)file_name.c_str();

	return ret;
}




const char* ModuleFileSystem::GetBasePath() const
{
	return PHYSFS_getBaseDir();
}

void ModuleFileSystem::GetEnumerateFiles(const char* dir, std::vector<std::string>& file_names) const
{
	if (Exists(dir))
	{
		char** files = PHYSFS_enumerateFiles(dir);

		for (uint i = 0; files[i]; i++)
		{
			file_names.push_back(files[i]);
		}

		PHYSFS_freeList(files);
	}
}