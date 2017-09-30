#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
//Methods
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();


	bool Init(JSON_Object* data);
	bool CleanUp();

	void SetTitle(const char* title);

	//Getters
	bool IsFullscreen() const;
	bool IsResizable() const;
	bool IsBorderless() const;
	bool IsFullScreenDesktop() const;
	int GetWidth() const;
	int GetHeight() const;

	//Setters
	void SetFullscreen(bool full);
	void SetResizable(bool res);
	void SetBorderless(bool border);
	void SetFullScreenDesktop(bool full_desk);
	void SetWidth(int w);
	void SetHeight(int h);

private:
	void SetWindowAttributes();

//Attributes
public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
private:

	bool win_fullscreen = false;
	bool win_resizable = true;
	bool win_borderless = false;
	bool win_fullscreen_desktop = false;

	int width = 1200;
	int height = 800;
};

#endif // __ModuleWindow_H__