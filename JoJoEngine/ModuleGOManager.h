#ifndef _MODULEGOMANAGER_H_
#define _MODULEGOMANAGER_H_

#include "Module.h"
#include <vector>

class GameObject;
class ComponentCamera;

class ModuleGOManager : public Module
{
public:
	ModuleGOManager(bool start_enabled = true);
	~ModuleGOManager();

	bool Start();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void FrustumCulling();

	void SetAsMainCamera(ComponentCamera* cam);
	
	GameObject* AddGameObject(const char* name, GameObject* parent = nullptr);
public:
	GameObject* root_GO;

private:
	std::vector<GameObject*> game_objects;
	ComponentCamera* main_camera = nullptr;
};


#endif //_MODULEGOMANAGER_H_
