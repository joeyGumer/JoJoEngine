#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"



class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	void AboutUs();

private:
	bool show_demo = false;
	
};
