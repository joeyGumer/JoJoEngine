#ifndef _MODULEFILESYSTEM_H_
#define _MODULEFILESYSTEM_H_

#include "Module.h"

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

	std::string GetFileExtension(const char* file) const;

	// Open for Read/Write
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	unsigned int Save(const char* file, const char* buffer, unsigned int size) const;

private:

};


#endif // !_MODULEFILESYSTEM_H_
