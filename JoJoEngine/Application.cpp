#include "Application.h"
#include "JSON\parson.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	//audio = new ModuleAudio(this, true);
	editor = new ModuleEditor();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	//physics = new ModulePhysics3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	//AddModule(audio);
	//AddModule(physics);
	
	// Scenes
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

bool Application::InitModules()
{
	bool ret = true;

	//LoadData from Config
	JSON_Value* config = json_parse_file("config.json");

	assert(config != nullptr);
	
	//Geting App data
	JSON_Object* data = json_value_get_object(config);

	//name = json_object_get_string(data, "name");
	//organization = json_object_get_string(data, "organization");

	//NOTE provisional data before having acces to JSON
	name = "JoJo Engine";
	organization = "CITM UPC";

	// Call Init() in all modules
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == true)
	{
		ret = (*i)->Init(data);
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