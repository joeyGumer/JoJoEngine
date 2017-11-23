#include "ModuleGOManager.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"

#include "GameObject.h"
#include "ComponentCamera.h"

//This module iterates all game functionality, editor functionality should not be added here
ModuleGOManager::ModuleGOManager(bool start_enabled) : Module(start_enabled)
{
	name = "go_manager";
}

ModuleGOManager::~ModuleGOManager()
{

}


bool ModuleGOManager::Start()
{
	root_GO = AddGameObject("Root");

	//NOTE: put to a separate function
	float3 point = { tree_size,tree_size, tree_size };
	AABB tree_aabb;
	tree_aabb.maxPoint = point;
	tree_aabb.minPoint = -point;
	game_tree.Create(tree_aabb);

	return true;
}
bool ModuleGOManager::CleanUp()
{
	RELEASE(root_GO);

	game_objects.clear();

	return true;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	//NOTE: editor functionality, should be out of this module
	//Camera Focus on Geometry
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_UP)
		FocusGameObject();

	return UPDATE_CONTINUE;
}
update_status ModuleGOManager::Update(float dt)
{
	//NOTE: there should always be a main_camera and frustum culling, if there's not, don't build the game
	//now we check for debugging and editor purposes
	if (main_camera && main_camera->frustum_culling)
		FrustumCulling();
	else
	{
		drawn_game_objects = game_objects;
	}

	return UPDATE_CONTINUE;
}
update_status ModuleGOManager::PostUpdate(float dt)
{
	
	for (uint i = 0, size = game_objects.size(); i < size; i++)
	{
		game_objects[i]->Update();
	}

	for (uint i = 0, size = drawn_game_objects.size(); i < size; i++)
	{
		drawn_game_objects[i]->Draw();
	}

	//NOTE: on second though, GO_manager should be reserved for GO iteration, but need to check this temporaly
	game_tree.Draw();

	return UPDATE_CONTINUE;
}

GameObject* ModuleGOManager::AddGameObject(const char* name, GameObject* parent)
{
	GameObject* GO = new GameObject(name, parent);

	game_objects.push_back(GO);

	return GO;
}

void ModuleGOManager::SetAsMainCamera(ComponentCamera* cam)
{
	main_camera = cam;
}
void ModuleGOManager::SetGoSelected(GameObject* s_go)
{
	selected_GO = s_go;
}

GameObject*  ModuleGOManager::GetGoSelected()const
{
	return selected_GO;
}

GameObject* ModuleGOManager::CastRayGO(LineSegment& ray, float3* hit_point)const
{
	GameObject* hit_go = nullptr;
	float distance = inf;

	//NOTE: have to be optimized with quadtree
	for (uint i = 0, size = game_objects.size(); i < size; i++)
	{
		GameObject* go = game_objects[i];

		if (go->HasMesh())
		{
			if (ray.Intersects(go->bb_axis))
			{
				float dist;
				if (go->GetCastRayDistance(ray, &dist, hit_point))
				{
					if (dist < distance)
					{
						distance = dist;
						hit_go = go;
					}
				}
			}
		}
	}

	return hit_go;

}

void ModuleGOManager::FocusGameObject() const
{
	
	if (selected_GO && selected_GO->HasMesh())
		App->camera->CenterCameraOnGeometry(selected_GO->bb_axis);
}

void ModuleGOManager::FrustumCulling()
{
	drawn_game_objects.clear();

	game_tree.IntersectCamera(drawn_game_objects, main_camera);
}

void ModuleGOManager::FillQuadTree()
{
	float3 point = { tree_size,tree_size, tree_size };
	AABB tree_aabb;
	tree_aabb.maxPoint = point;
	tree_aabb.minPoint = -point;
	game_tree.Create(tree_aabb);


	for (int i = 0, size = game_objects.size(); i < size; i++)
	{
		//Note: For now, we don't check for static GO
		//If parent is static children should be too
		if (game_objects[i]->HasMesh())
		{
			game_tree.Insert(game_objects[i]);
		}
	}
}