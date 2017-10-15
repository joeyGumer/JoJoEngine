#ifndef __WINCONFIGURATION_H__
#define __WINCONFIGURATION_H__

#include "EditorWindow.h"
#include "Bar.h"
#include "OpenGl.h"

class WinConfiguration: public EditorWindow
{
public:
	WinConfiguration();
	~WinConfiguration();

private:
	void Start();
	void Update();
	void CleanUp();


	void TabApplication();
	void TabWindow();
	void TabRenderer();
	void TabHardware();
	void TabCamera();
	void TabInput();

private:
	//FPS graphs
	int slider_fps;
	Bar fps;
	Bar ms;

	//Hardware specs
	std::string caps;
	GLint total_vram;
	GLint initial_available_vram;
	GLint available_vram;
	GLint usage_vram;
};

#endif __WINDOWCONFIGURATION_H__

