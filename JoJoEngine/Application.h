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

//NOTE: change the includes to the cpp
using namespace std; 

class Application
{
public:
	
	ModuleFileSystem* fs;
	ModuleWindow* window;
	ModuleInput* input;
	//ModuleAudio* audio;
	ModuleEditor* editor;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleSceneEdit* level;
	//ModulePhysics3D* physics;

private:

	Timer	ms_timer;
	float	dt;
	list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	const std::string GetSDLVersion();

	//Getters
	std::string GetName() const;
	std::string GetOrganization() const;

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