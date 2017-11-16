#pragma once
#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Math.h"
#include "Globals.h"
#include <vector>
#include <list>

class GameObject;
class Quadtree;

struct QuadNode
{
	QuadNode(Quadtree* tr, AABB& lim, uint i) : tree(tr), limits(lim) ,id(i){}

	void Subdivide();
	bool ToDivide();
	void Insert(GameObject* go);
	void Draw();

	AABB limits;

	QuadNode* parent = nullptr;
	std::vector<QuadNode*> children;
	bool subdivided = false;

	std::vector<GameObject*> game_objects;
	uint go_limit = 1;

	uint id = 0;
	Quadtree* tree;
};

class Quadtree
{
public:
	void Create(AABB& limits);
	void Clear();
	void Insert(GameObject* go);
	void Remove(GameObject* go);
	//void Intersect(std::vector<GameObject*>&, PRIMITIVE);

	uint AssignId() { return ++id; }
	void Draw();
public:
	QuadNode* root_node;

	std::list<QuadNode*> regions;

	uint id = 0;
};


#endif // !_QUADTREE_H_
