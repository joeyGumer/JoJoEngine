#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Imgui\imgui.h"
#include "EditorWindow.h"

class Console :  public EditorWindow
{
public:
	Console();
	~Console();

	void Start();
	void Update();
	void CleanUp();

	void Write(const char* text);

private:
	ImGuiTextBuffer		buffer;
	bool                ScrollToBottom;
};

extern Console* console;

#endif
