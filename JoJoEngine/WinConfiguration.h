#ifndef __WINCONFIGURATION_H__
#define __WINCONFIGURATION_H__

#include "EditorWindow.h"
#include "Module.h"
#include "Bar.h"

class WinConfiguration: public EditorWindow
{
public:
	WinConfiguration();
	~WinConfiguration();

	void Start();
	void Update();

private:
	void TabApplication();
	void TabWindow();
	void TabRenderer();

private:
	Bar fps;
	Bar ms;
};

#endif __WINDOWCONFIGURATION_H__

