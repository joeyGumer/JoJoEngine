#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"

using namespace std; 

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	//ModuleAudio* audio;
	ModuleEditor* editor;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
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

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	bool InitModules();
};