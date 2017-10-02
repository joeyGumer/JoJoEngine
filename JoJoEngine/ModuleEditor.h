#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include <vector>

class EditorWindow;
class WinConfiguration;

class ModuleEditor : public Module
{
public:
	ModuleEditor(bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data);

	void Draw() const;

private:
	void AboutUs();
	void AddWindow(EditorWindow* win);

private:
	bool show_demo = false;

	std::vector<EditorWindow*> editor_windows;
	WinConfiguration* configuration = nullptr;

	
};
