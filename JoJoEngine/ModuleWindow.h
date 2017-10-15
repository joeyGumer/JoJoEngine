#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

enum WINDOW_MODE
{
	RESIZABLE,
	FULL_DESKTOP,
	FULLSCREEN
};

class ModuleWindow : public Module
{
//Methods
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data)const;

	bool Init();
	update_status Update(float dt);
	bool CleanUp();	

	//Getters
	const WINDOW_MODE GetWindowMode() const;
	const int GetWidth() const;
	const int GetHeight() const;
	const int GetScreenSize() const;
	const bool GetVsync() const;

	//Setters
	void SetTitle(const char* title);
	void SetWindowMode(const int& m);
	void SetWidth(const int& w);
	void SetHeight(const int& h);
	void SetScreenSize(const int& size);
	void SetVsync(const bool& vs);

private:
	void SetWindowAttributes();

//Attributes
public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
private:
	std::string title;
	int width;
	int height;	
	int screen_size;
	bool vsync;
	WINDOW_MODE win_mode;
};

#endif // __ModuleWindow_H__