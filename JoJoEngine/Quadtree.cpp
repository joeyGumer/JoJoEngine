#include "Quadtree.h"

//NOTE: not sure if it's needed to add the render, or put it in GO_manager
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "ComponentCamera.h"

QuadNode::QuadNode(Quadtree* tr, AABB& lim, uint i) : tree(tr), limits(lim), id(i)
{
	for (uint i = 0; i < 4; i++)
	{
		children[i] = nullptr;
	}
}

QuadNode::~QuadNode()
{
	if (subdivided)
	{
		for (int i = 0; i < 4; i++)
		{
			RELEASE(children[i]);
		}
	}
	game_objects.clear();
}

void QuadNode::Insert(GameObject* go)
{
		game_objects.push_back(go);

		if (ToDivide())
		{
			Subdivide();
		}

		if (subdivided)
			RedistributeGoChilds();
		/*for (uint i = 0, size = children.size(); i < size; i++)
		{
			children[i]->Insert(go);
		}*/
	
}
void QuadNode::Subdivide()
{ 
	//Ric's subdivide // We need to subdivide this node ...
	float3 size(limits.Size());
	float3 new_size(size.x*0.5f, size.y, size.z*0.5f);

	float3 center(limits.CenterPoint());
	float3 new_center(center);
	AABB aabb;

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			new_center.x = center.x + size.x * 0.25f;
			new_center.z = center.z + size.z * 0.25f;
			break;
		case 1:
			new_center.x = center.x + size.x * 0.25f;
			new_center.z = center.z - size.z * 0.25f;
			break;
		case 2:
			new_center.x = center.x - size.x * 0.25f;
			new_center.z = center.z - size.z * 0.25f;
			break;
		case 3:
			new_center.x = center.x - size.x * 0.25f;
			new_center.z = center.z + size.z * 0.25f;
			break;
		}
		
		aabb.SetFromCenterAndSize(new_center, new_size);
		children[i] = new QuadNode(tree, aabb, tree->AssignId());
	}

	subdivided = true;
}

void QuadNode::RedistributeGoChilds()
{
	for (std::list<GameObject*>::iterator go = game_objects.begin(); go != game_objects.end(); )
	{
		bool go_intersects[4];

		bool all_intersects = true;
		for (int i = 0; i < 4; i++)
		{
			if (!(go_intersects[i] = children[i]->limits.Intersects((*go)->bb_axis)))
				all_intersects = false;
		}

		if (!all_intersects)
		{
			for (int i = 0; i < 4; i++)
			{
				if (go_intersects[i])
					children[i]->Insert(*go);
			}

			go = game_objects.erase(go);
		}
		else
			go++;
	}
}


//NOTE: later i'll do it with the regions
void QuadNode::Draw()
{
	App->renderer3D->DrawAABB(&limits);

	if (subdivided)
	{
		for (int i = 0; i < 4; i++)
		{
			children[i]->Draw();
		}
	}
}
bool QuadNode::ToDivide()
{
	float hx = limits.maxPoint.x - limits.minPoint.x;
	float hz = limits.maxPoint.z - limits.minPoint.z;
	return (!subdivided && (game_objects.size() > NODE_GO_LIMIT && ( hx > (NODE_LIMIT_SIZE) || hz > (NODE_LIMIT_SIZE))));
}



//----------------------------------------------Quadtree
void Quadtree::Create(AABB& limits)
{
	Clear();

	root_node = new QuadNode(this, limits, id);

	//regions.push_back(root_node);
}

void Quadtree::Clear()
{
	if(root_node)
		RELEASE(root_node);
}

void Quadtree::Insert(GameObject* go)
{
	//Get position
	if (go->HasMesh())
	{
		//For now, i will recursevily add a gameobject
		if (root_node->limits.Intersects(go->bb_axis))
		{
			root_node->Insert(go);
		}
	}
}
void Quadtree::Remove(GameObject* go)
{

}

void Quadtree::Draw()
{
	root_node->Draw();
}

//NOTE: with this method, GameObjects will be repeated, use it anyways?
//Intersection functions
template<typename TYPE>
inline void QuadNode::CollectIntersections(std::vector<GameObject*>& go_list, const TYPE& primitive) const
{
	if (primitive.Intersects(limits))
	{
		for (std::list<GameObject*>::const_iterator it = game_objects.begin(); it != game_objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->bb_axis))
				go_list.push_back(*it);

		}
		for (int i = 0; i < 4; ++i)
			if (children[i] != nullptr) children[i]->CollectIntersections(go_list, primitive);

	}
}

template<typename TYPE>
inline void Quadtree::Intersect(std::vector<GameObject*>& objects, const TYPE& primitive) const
{
	root_node->CollectIntersections();
}

//Collect intersections with the method created for Frustum intersection faster than the one at MathGeoLib
void QuadNode::CollectCameraIntersections(std::vector<GameObject*>& go_list, const ComponentCamera* cam) const
{
	if (cam->IntersectAABB(limits))
	{
		for (std::list<GameObject*>::const_iterator it = game_objects.begin(); it != this->game_objects.end(); ++it)
		{
			if (cam->CullGameObject((*it)))
				go_list.push_back(*it);

		}
		for (int i = 0; i < 4; ++i)
		{
			if (children[i] != nullptr)
				children[i]->CollectCameraIntersections(go_list, cam);
		}
	}
}

void Quadtree::IntersectCamera(std::vector<GameObject*>& objects, const ComponentCamera* cam) const
{

	root_node->CollectCameraIntersections(objects, cam);

}