#ifndef __WINCONFIGURATION_H__
#define __WINCONFIGURATION_H__

#include "EditorWindow.h"
#include "Module.h"

class WinConfiguration: public EditorWindow
{
public:
	WinConfiguration();
	~WinConfiguration();

	void Update();

private:
	void TabApplication();
	void TabWindow();
	void TabRenderer();
	
};

#endif __WINDOWCONFIGURATION_H__

