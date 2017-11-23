#ifndef _MODULEGOMANAGER_H_
#define _MODULEGOMANAGER_H_

#include "Module.h"

//NOTE: direct or forward declaration?
#include "Quadtree.h"

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


	//-----------

	void SetAsMainCamera(ComponentCamera* cam);
	void SetGoSelected(GameObject* s_go);

	GameObject* GetGoSelected()const;
	GameObject* CastRayGO(LineSegment& ray, float3* hit_point) const;
	GameObject* AddGameObject(const char* name, GameObject* parent = nullptr);

	void FrustumCulling();
	void FocusGameObject() const;
	void FillQuadTree();

	

public:
	GameObject* root_GO;

	//NOTE: temporaly public
	Quadtree game_tree;
private:
	GameObject* selected_GO = nullptr;
	float tree_size = 100.0;

	std::vector<GameObject*> game_objects;
	std::vector<GameObject*> drawn_game_objects;
	ComponentCamera* main_camera = nullptr;
};


#endif //_MODULEGOMANAGER_H_
