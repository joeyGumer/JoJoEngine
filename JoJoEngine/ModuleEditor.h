#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include <vector>

class EditorWindow;
class WinConfiguration;
class WinProperties;
class ImGuiDemo;

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

	const bool IsActive() const;

private:
	void AboutUs();
	void AddWindow(EditorWindow* win);
	void TurnOnOff();

	void CreateWindows();

private:
	bool editor_active;

	std::vector<EditorWindow*> editor_windows;
	WinConfiguration* configuration = nullptr;
	WinProperties* properties = nullptr;
	ImGuiDemo* demo = nullptr;	
};
