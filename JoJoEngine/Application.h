#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"

class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleEditor;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModulePhysiscs3D;
class ModuleSceneEdit;
class ModuleFileSystem;
class ModuleFBXLoader;

//NOTE: change the includes to the cpp
using namespace std; 

class Application
{
public:
	
	ModuleFileSystem* fs;
	ModuleFBXLoader* fbx;
	ModuleWindow* window;
	ModuleInput* input;
	//ModuleAudio* audio;
	ModuleEditor* editor;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleSceneEdit* level;
	//ModulePhysics3D* physics;

private:

	list<Module*> list_modules;

	//Timers
	Timer	startup_timer;
	Timer	frame_timer;
	Timer	last_frame_timer;
	int		frame_count;
	int		last_frame_count;
	int		prev_last_frame_count;
	float	last_frame_ms;
	float	capped_ms;

	float	dt;
	

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	const std::string GetSDLVersion();

	//Getters
	const std::string GetName() const;
	const std::string GetOrganization() const;
	const int GetFPS();
	const int GetMs();
	const bool SecCounter();

	//Setters
	void SetName(const char* str);
	void SetOrganization(const char* str);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	bool InitModules();

	bool LoadConfig();
	bool SaveConfig();
private:
	std::string name;
	std::string organization;
};

extern Application* App;