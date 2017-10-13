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

	void Start();
	void Update();
	void CleanUp();

private:
	void TabApplication();
	void TabWindow();
	void TabRender();
	void TabHardware();

private:
	//FPS graphs
	int slider_fps;
	Bar fps;
	Bar ms;

	//Hardware specs
	std::string caps;
	GLint total_vram;
	GLint available_vram;
	GLint usage_vram;
};

#endif __WINDOWCONFIGURATION_H__

