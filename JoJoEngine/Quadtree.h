#pragma once
#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Math.h"
#include "Globals.h"
#include <vector>
#include <list>

class GameObject;
class Quadtree;

#define NODE_GO_LIMIT 1
#define NODE_LIMIT_SIZE 10.0f


struct QuadNode
{
	QuadNode(Quadtree* tr, AABB& lim, uint i) : tree(tr), limits(lim) ,id(i){}
	~QuadNode();

	void Subdivide();
	bool ToDivide();
	void RedistributeGoChilds();
	void Insert(GameObject* go);
	void Draw();

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;

	AABB limits;
	QuadNode* parent = nullptr;
	QuadNode* children[4];
	bool subdivided = false;

	std::list<GameObject*> game_objects;
	//uint go_limit = 1;

	uint id = 0;
	Quadtree* tree;
};

class Quadtree
{

public:

	Quadtree() :root_node(nullptr) {}

	void Create(AABB& limits);
	void Clear();
	void Insert(GameObject* go);
	void Remove(GameObject* go);

	template<typename TYPE>
	void Intersect(std::vector<GameObject*>& objects, const TYPE& primitive) const;

	uint AssignId() { return ++id; }
	void Draw();
public:

	QuadNode* root_node = nullptr;

	std::list<QuadNode*> regions;

	uint id = 0;
};


//Intersection functions
template<typename TYPE>
inline void QuadNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const
{
	if (primitive.Intersects(box))
	{
		for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->global_bbox))
				objects.push_back(*it);

		}
		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, primitive);

	}
}

template<typename TYPE>
inline void Quadtree::Intersect(std::vector<GameObject*>& objects, const TYPE& primitive) const
{
	root_node->CollectIntersections();
}

#endif // !_QUADTREE_H_
