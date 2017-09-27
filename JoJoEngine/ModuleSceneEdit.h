#ifndef __MODULESCENEEDIT_H__
#define __MODULESCENEEDIT_H__

#include "Module.h"
#include "Globals.h"

class ModuleSceneEdit : public Module
{
public:
	ModuleSceneEdit(bool start_enabled = true);
	~ModuleSceneEdit();

	bool Start();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void Draw();
};

#endif  __MODULESCENEEDIT_H__

