#ifndef __WINCONSOLE_H__
#define __WINCONSOLE_H__

#include "Imgui\imgui.h"
#include "EditorWindow.h"

class WinConsole :  public EditorWindow
{
public:
	WinConsole();
	~WinConsole();

	void Start();
	void Update();
	void CleanUp();

	void Write(const char* text);

private:
	ImGuiTextBuffer		buffer;
	bool                ScrollToBottom;
};

extern WinConsole* console;

#endif
