#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleSceneEdit.h"
#include "ModuleFileSystem.h"
#include "ModuleFBXLoader.h"

#include "JSON\parson.h"

Application::Application()
{
	fs = new ModuleFileSystem();
	fbx = new ModuleFBXLoader();
	window = new ModuleWindow();
	input = new ModuleInput();
	//audio = new ModuleAudio(this, true);
	editor = new ModuleEditor();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	level = new ModuleSceneEdit();
	//physics = new ModulePhysics3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(fs);
	AddModule(fbx);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	//AddModule(audio);
	//AddModule(physics);	
	// Scenes
	AddModule(level);
	AddModule(editor);
	// Renderer last!
	AddModule(renderer3D);

	frame_count = 0;
	last_frame_count = 0;
	prev_last_frame_count = 0;
	last_frame_ms = 0.0f;
	capped_ms = -1.0f;
	dt = 0.0f;
}

Application::~Application()
{
	list<Module*>::reverse_iterator i = list_modules.rbegin();

	while (i != list_modules.rend())
	{
		delete (*i);
		++i;
	}
}

bool Application::Init()
{
	bool ret = InitModules();

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	list<Module*>::iterator i = list_modules.begin();

	while(i != list_modules.end() && ret == true)
	{
		ret = (*i)->Start();
		++i;
	}
	
	startup_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_frame_count++;

	dt = frame_timer.ReadSec();
	frame_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (SecCounter())
	{
		last_frame_timer.Start();
		prev_last_frame_count = last_frame_count;
		last_frame_count = 0;
	}

	last_frame_ms = frame_timer.Read();


	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->PreUpdate(dt);
		++i;
	}

	i = list_modules.begin();

	while(i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->Update(dt);
		++i;
	}

	i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->PostUpdate(dt);
		i++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	//Save all config before closing
	//NOTE: do all json load/save assertions
	SaveConfig();

	list<Module*>::reverse_iterator i = list_modules.rbegin();

	while (i != list_modules.rend() && ret == true)
	{
		ret = (*i)->CleanUp();
		++i;
	}


	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

bool Application::LoadConfig()
{
	bool ret = true;

	//LoadData from Config
	JSON_Value* config = json_parse_file("config.json");

	assert(config != nullptr);

	//Geting App data
	JSON_Object* data = json_value_get_object(config);
	JSON_Object* app_data = json_object_get_object(data, "App");

	name = json_object_get_string(app_data, "name");
	organization = json_object_get_string(app_data, "organization");

	// Call LoadConfig() in all modules
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == true)
	{
		JSON_Object* module_config = json_object_get_object(app_data, (*i)->name.c_str());
		ret = (*i)->LoadConfig(module_config);
		++i;
	}


	return ret;
}

bool Application::SaveConfig()
{
	bool ret = true;

	JSON_Value* file = json_parse_file("config.json");
 	JSON_Object* config = json_value_get_object(file);
	JSON_Object* app_config = json_object_get_object(config, "App");

	json_object_set_string(config, "name", name.c_str());
	json_object_set_string(config, "organization", organization.c_str());	
	
	// Call SaceConfig() in all modules
	list<Module*>::iterator i = list_modules.begin();
	while (i != list_modules.end() && ret == true)
	{
		JSON_Object* module_config = json_object_get_object(app_config, (*i)->name.c_str());
		ret = (*i)->SaveConfig(module_config);
		++i;
	}

	json_serialize_to_file_pretty(file, "config.json");

	return ret;
}
bool Application::InitModules()
{
	bool ret = true;

	//Loading all configuration variables before config
	LoadConfig();

	// Call Init() in all modules
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == true)
	{
		ret = (*i)->Init();
		++i;
	}

	return ret;
}

const std::string Application::GetSDLVersion()
{
	SDL_version version;
	SDL_VERSION(&version);
	std::string ret = "SDL ";
	ret += std::to_string(version.major);
	ret += "." + std::to_string(version.minor);
	ret += "." + std::to_string(version.patch);

	return ret;
}

//Getters
//NOTE: maybe to harsh to return a direct string
const std::string Application::GetName() const
{
	return name;
}

const std::string Application::GetOrganization() const
{
	return organization;
}

const int Application::GetFPS() const
{
	return prev_last_frame_count;
}

const int Application::GetMs() const
{
	return last_frame_ms;
}

const bool Application::SecCounter() const
{
	return (last_frame_timer.Read() > 1000);
}

//Setters
void Application::SetName(const char* str)
{
	name = str;
}

void Application::SetOrganization(const char* str)
{
	organization = str;
}

void Application::SetMaxFPS(const uint fps)
{
	if (fps != 0)
		capped_ms = 1000 / fps;
	else
		capped_ms = 1000 / -1;
}