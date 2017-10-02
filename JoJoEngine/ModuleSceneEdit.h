#ifndef __MODULESCENEEDIT_H__
#define __MODULESCENEEDIT_H__

#include "Module.h"
#include "Globals.h"


class PCube;
class PCylinder;
class PWiredPlane;
class PPlane;
class PSphere;

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

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data);

	void Draw();

private:
	//Example primitives
	PWiredPlane* wplane;
	//NOTE: temporal

	PCube* ex_cube;
	PCylinder* ex_cylinder;
	PPlane* ex_plane;
	PSphere* ex_sphere;
};

#endif  __MODULESCENEEDIT_H__

