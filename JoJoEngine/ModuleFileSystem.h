#ifndef _MODULEFILESYSTEM_H_
#define _MODULEFILESYSTEM_H_

#include "Module.h"
#include <vector>
#include <string>

struct SDL_RWops;


int close_sdl_rwops(SDL_RWops *rw);

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem();

	// Destructor
	virtual ~ModuleFileSystem();

	// Called before render is available
	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data) const;

	// Called before quitting
	bool CleanUp();

	// Utility functions
	bool AddPath(const char* path_or_zip, const char* mount_point = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;

	const char* GetSaveDirectory() const
	{
		return "save/";
	}
	const char* GetBasePath() const;

	bool CreateDirectoryFile(const char* directory);

	std::string GetFileExtension(const char* file) const;
	std::string GetFileName(const char* path, std::string& output) const;

	void GetEnumerateFiles(const char* dir, std::vector<std::string>& file_names) const;

	// Open for Read/Write
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	unsigned int Save(const char* file, const char* buffer, unsigned int size) const;
	bool SaveUnique(const char* file, const char* buffer, unsigned int size, const char* path, const char* extension, std::string& output_filename);


private:

};


#endif // !_MODULEFILESYSTEM_H_
