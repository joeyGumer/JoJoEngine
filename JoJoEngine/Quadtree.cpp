#include "Quadtree.h"

//NOTE: not sure if it's needed to add the render, or put it in GO_manager
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"

void QuadNode::Insert(GameObject* go)
{
	if (limits.Intersects(go->bb_axis))
	{
		game_objects.push_back(go);

		if (ToDivide())
		{
			Subdivide();
		}

		for (uint i = 0, size = children.size(); i < size; i++)
		{
			children[i]->Insert(go);
		}
	}

	
}
void QuadNode::Subdivide()
{
	float hx, hy;
	float3 min = limits.minPoint;
	float3 max = limits.maxPoint;

	hx = (max.x - min.x) / 2;
	hy = (max.z - min.z) / 2;

	for (uint i = 0; i < 4; i++)
	{
		AABB aabb;

		switch (i)
		{
		case 0:
			aabb.maxPoint = max - float3(hx, 0, 0);
			aabb.minPoint = min + float3(0, 0, hy);
			break;
		case 1:
			aabb.maxPoint = max;
			aabb.minPoint = max - float3(hx, 0, hy);
			break;
		case 2:
			aabb.maxPoint = max - float3(hx, 0, hy);
			aabb.minPoint = min;
			break;
		case 3:
			aabb.maxPoint = min + float3(hx, 0, 0);
			aabb.minPoint = max - float3(0, 0, hy);
			break;
		}

		QuadNode* child = new QuadNode(tree, aabb, tree->AssignId());

		children.push_back(child);
	}
	subdivided = true;
}

//NOTE: later i'll do it with the regions
void QuadNode::Draw()
{
	App->renderer3D->DrawAABB(&limits);

	for (int i = 0, size = children.size(); i < size; i++)
	{
		children[i]->Draw();
	}
}
bool QuadNode::ToDivide()
{
	return (!subdivided && game_objects.size() > go_limit);
}

void Quadtree::Create(AABB& limits)
{
	root_node = new QuadNode(this, limits, id);

	regions.push_back(root_node);
}

void Quadtree::Clear()
{

}
void Quadtree::Insert(GameObject* go)
{
	//Get position
	//For now, i will recursevily add a gameobject
	root_node->Insert(go);

	/*
	std::list<QuadNode>::iterator i = regions.begin();
	for (; i != regions.end(); i++)
	{
		QuadNode* node = &(*i);
		if (node->limits.Intersects(go->bb_axis))
		{
			node->game_objects.push_back(go);

			if (node->IsFull())
			{
				node->Subdivide();
				//regions.erase(i)
			}
		}
	}
*/
}
void Quadtree::Remove(GameObject* go)
{

}
//void Quadtree::Intersect(std::vector<GameObject*>&, PRIMITIVE);

void Quadtree::Draw()
{
	root_node->Draw();
}