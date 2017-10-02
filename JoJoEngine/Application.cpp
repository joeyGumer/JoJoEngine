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

#include "JSON\parson.h"

Application::Application()
{
	//fs = new ModuleFileSystem();
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
	//AddModule(fs);
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
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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

	//NOTE provisional data before having acces to JSON
	//name = "JoJo Engine";
	//organization = "CITM UPC";

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

	JSON_Value* file = json_value_init_object();
	JSON_Object* config = json_value_get_object(file);

	json_object_dotset_string(config, "App.name", name.c_str());

	JSON_Object* app_config = json_object_get_object(config, "App");

	json_object_set_string(config, "organization", organization.c_str());
	
	//NOTE: creating each module object here or at the own SaveConfig module?
	//RE NOTE: create a dummy value or an id to index the module
	// Call SaceConfig() in all modules
	list<Module*>::iterator i = list_modules.begin();
	while (i != list_modules.end() && ret == true)
	{
		ret = (*i)->SaveConfig(app_config);
		++i;
	}

	json_value_free(file);

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
std::string Application::GetName() const
{
	return name;
}

std::string Application::GetOrganization() const
{
	return organization;
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