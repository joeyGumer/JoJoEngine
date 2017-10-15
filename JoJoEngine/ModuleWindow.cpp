#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "JSON\parson.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;

	name = "window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width *= screen_size;
		height *= screen_size;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SetWindowAttributes();

		switch (win_mode)
		{
			case WINDOW_MODE::RESIZABLE: flags |= SDL_WINDOW_RESIZABLE; break;
			case WINDOW_MODE::FULL_DESKTOP: flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; break;
			case WINDOW_MODE::FULLSCREEN: flags |= SDL_WINDOW_FULLSCREEN; break;
		}

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Delete OpenGl context for SDL window
	SDL_GL_DeleteContext(window);

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

//Getters
const WINDOW_MODE  ModuleWindow::GetWindowMode() const
{
	return win_mode;
}

const int ModuleWindow::GetWidth() const
{
	return width;
}
const int ModuleWindow::GetHeight() const
{
	return height;
}

const int ModuleWindow::GetScreenSize() const
{
	return screen_size;
}

const bool ModuleWindow::GetVsync() const
{
	return vsync;
}

//Setters
void ModuleWindow::SetTitle(const char* t)
{
	title = t;
	SDL_SetWindowTitle(window, title.c_str());
}

void ModuleWindow::SetWindowMode(const int& m)
{
	switch (m)
	{
		case WINDOW_MODE::RESIZABLE: 
		{
			win_mode = RESIZABLE;
			SDL_SetWindowFullscreen(window, SDL_WINDOW_RESIZABLE);
			break;
		};
		case WINDOW_MODE::FULL_DESKTOP:
		{
			win_mode = FULL_DESKTOP;
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			break;
		};
		case WINDOW_MODE::FULLSCREEN:
		{
			win_mode = FULLSCREEN;
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			break;
		};
	}	
}

void ModuleWindow::SetWidth(const int& w)
{
	width = w;
	SDL_SetWindowSize(window, width, height);
}
void ModuleWindow::SetHeight(const int& h)
{
	height = h;
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::SetScreenSize(const int& size)
{
	screen_size = size;
}
void ModuleWindow::SetVsync(const bool& vs)
{
	vsync = vs;
}

//---------------
void ModuleWindow::SetWindowAttributes()
{
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
}


//Called to load configuration variables
bool ModuleWindow::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	title = json_object_get_string(data, "title");
	width = json_object_get_number(data, "width");
	height = json_object_get_number(data, "height");
	screen_size = json_object_get_number(data, "screen_size");
	vsync = json_object_get_boolean(data, "vsync");

	switch ((int)json_object_get_number(data, "window_mode"))
	{
		case WINDOW_MODE::RESIZABLE: win_mode = RESIZABLE; break;
		case WINDOW_MODE::FULL_DESKTOP: win_mode = FULL_DESKTOP; break;
		case WINDOW_MODE::FULLSCREEN: win_mode = FULLSCREEN; break;
	}
	return ret;
}

bool ModuleWindow::SaveConfig(JSON_Object* data)
{
	bool ret = true;
	
	json_object_set_number(data, "width", width);
	json_object_set_number(data, "height", height);
	json_object_set_number(data, "screen_size", screen_size);
	json_object_set_number(data, "window_mode", (int)win_mode);
	json_object_set_boolean(data, "vsync", vsync);
	json_object_set_string(data, "title", title.c_str());

	return ret;
}