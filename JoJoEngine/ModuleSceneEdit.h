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

	GameObject* AddGameObject(const char* name, GameObject* parent = nullptr);
public:
	GameObject* root_GO;
private:

	std::vector<GameObject*> game_objects;
	

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

