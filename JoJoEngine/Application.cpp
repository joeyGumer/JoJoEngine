#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneEdit.h"
#include "ModuleFileSystem.h"
#include "ModuleFBXLoader.h"
#include "ModuleGOManager.h"

#include "JSON\parson.h"

Application::Application()
{
	fs = new ModuleFileSystem();
	fbx = new ModuleFBXLoader();
	window = new ModuleWindow();
	input = new ModuleInput();
	editor = new ModuleEditor();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	level = new ModuleSceneEdit();
	go_manager = new ModuleGOManager();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(fs);
	AddModule(fbx);
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	// Scenes
	AddModule(level);
	AddModule(go_manager);
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
	
	int i = list_modules.size() - 1;
	while (i >= 0)
	{
		delete list_modules[i];
		--i;
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = InitModules();

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");


	uint size = list_modules.size();
	uint i = 0;
	while(i < size)
	{
		ret = list_modules[i]->Start();
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
	
	uint size = list_modules.size();
	uint i = 0;
	while( i < size && ret == UPDATE_CONTINUE)
	{
		ret = list_modules[i]->PreUpdate(dt);
		++i;
	}

	i = 0;

	while(i < size && ret == UPDATE_CONTINUE)
	{
		ret = list_modules[i]->Update(dt);
		++i;
	}

	i = 0;

	while (i < size && ret == UPDATE_CONTINUE)
	{
		ret = list_modules[i]->PostUpdate(dt);
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

	
	int i = list_modules.size() - 1;

	while (i >= 0 && ret == true)
	{
		ret = list_modules[i]->CleanUp();
		--i;
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
	//NOTE: will need to be changed
	JSON_Value* config = json_parse_file("Game/config.json");

	assert(config != nullptr);

	//Geting App data
	JSON_Object* data = json_value_get_object(config);
	JSON_Object* app_data = json_object_get_object(data, "App");

	name = json_object_get_string(app_data, "name");
	organization = json_object_get_string(app_data, "organization");

	// Call LoadConfig() in all modules
	uint size = list_modules.size();
	uint i = 0;
	while (i < size && ret == true)
	{
		JSON_Object* module_config = json_object_get_object(app_data, list_modules[i]->name.c_str());
		ret = list_modules[i]->LoadConfig(module_config);
		++i;
	}

	json_value_free(config);

	return ret;
}

bool Application::SaveConfig() 
{
	bool ret = true;

	JSON_Value* file = json_parse_file("Game/config.json");
 	JSON_Object* config = json_value_get_object(file);
	JSON_Object* app_config = json_object_get_object(config, "App");

	json_object_set_string(config, "name", name.c_str());
	json_object_set_string(config, "organization", organization.c_str());	
	
	// Call SaceConfig() in all modules
	uint size = list_modules.size();
	uint i = 0;
	while (i < size && ret == true)
	{
		JSON_Object* module_config = json_object_get_object(app_config, list_modules[i]->name.c_str());
		ret = list_modules[i]->SaveConfig(module_config);
		++i;
	}

	json_serialize_to_file_pretty(file, "config.json");
	json_value_free(file);
	
	return ret;
}
bool Application::InitModules()
{
	bool ret = true;

	//Loading all configuration variables before config
	LoadConfig();

	// Call Init() in all modules
	uint size = list_modules.size();
	uint i = 0;
	while (i < size && ret == true)
	{
		ret = list_modules[i]->Init();
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