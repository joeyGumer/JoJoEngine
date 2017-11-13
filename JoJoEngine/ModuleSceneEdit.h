#ifndef __MODULESCENEEDIT_H__
#define __MODULESCENEEDIT_H__

#include "Module.h"
#include "Globals.h"

#include <vector>


class PCube;
class PCylinder;
class PWiredPlane;
class PPlane;
class PSphere;
class PAxis;
class GameObject;

//NOTE: not sure to add this here
class ComponentCamera;

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
	bool SaveConfig(JSON_Object* data)const;

	void Draw();
public:

	
private:

	//Base geometry of the scene
	PWiredPlane* wplane;
	PAxis* world_axis;

	//Primitives for example
	PCube* ex_cube;
	PCylinder* ex_cylinder;
	PPlane* ex_plane;
	PSphere* ex_sphere;

};

#endif  __MODULESCENEEDIT_H__

