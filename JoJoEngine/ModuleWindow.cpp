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
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SetWindowAttributes();

		if(win_fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(win_resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(win_borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(win_fullscreen_desktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

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

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

//Getters
bool  ModuleWindow::IsFullscreen() const
{
	return win_fullscreen;
}
bool  ModuleWindow::IsResizable() const
{
	return win_resizable;
}
bool  ModuleWindow::IsBorderless() const
{
	return win_borderless;
}
bool  ModuleWindow::IsFullScreenDesktop() const
{
	return win_fullscreen_desktop;
}

int ModuleWindow::GetWidth() const
{
	return width;
}
int ModuleWindow::GetHeight() const
{
	return height;
}

//Setters
//NOTE: look how to make the screen attributes automatic change
void ModuleWindow::SetFullscreen(bool full)
{
	win_fullscreen = full;
}
void ModuleWindow::SetResizable(bool res)
{
	win_resizable = res;
}
void ModuleWindow::SetBorderless(bool border)
{
	win_borderless = border;
}
void ModuleWindow::SetFullScreenDesktop(bool full_desk)
{
	win_fullscreen_desktop = full_desk;
}

void ModuleWindow::SetWidth(int w)
{
	width = w;
}
void ModuleWindow::SetHeight(int h)
{
	height = h;
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

	width = json_object_get_number(data, "width");
	height = json_object_get_number(data, "height");

	win_fullscreen = json_object_get_boolean(data, "fullscreen");
	win_resizable = json_object_get_boolean(data, "resizable");
	win_borderless = json_object_get_boolean(data, "borderless");
	win_fullscreen_desktop = json_object_get_boolean(data, "fullscreen_desktop");

	return ret;
}

bool ModuleWindow::SaveConfig(JSON_Object* data)
{
	bool ret = true;

	json_object_dotset_number(data, "window.width", width);
	
	JSON_Object* window_config = json_object_get_object(data, "window");
	
	json_object_set_number(window_config, "height", height);

	json_object_set_boolean(window_config, "fullscreen", win_fullscreen);
	json_object_set_boolean(window_config, "resizable", win_resizable);
	json_object_set_boolean(window_config, "borderless", win_borderless);
	json_object_set_boolean(window_config, "fullscreen_desktop", win_fullscreen_desktop);

	//NOTE: add values : screen_size, vsync and title



	return ret;
}