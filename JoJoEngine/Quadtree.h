#pragma once
#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Math.h"
#include "Globals.h"
#include <vector>
#include <list>

class GameObject;
class Quadtree;
class ComponentCamera;


#define NODE_GO_LIMIT 1
#define NODE_LIMIT_SIZE 10.0f


struct QuadNode
{
	QuadNode(Quadtree* tr, AABB& lim, uint i);
	~QuadNode();

	void Subdivide();
	bool ToDivide();
	void RedistributeGoChilds();
	void Insert(GameObject* go);
	void Draw();

	void CollectCameraIntersections(std::vector<GameObject*>& objects, const ComponentCamera* cam) const;
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

	void IntersectCamera(std::vector<GameObject*>& objects, const ComponentCamera* cam) const;

	template<typename TYPE>
	void Intersect(std::vector<GameObject*>& objects, const TYPE& primitive) const;
	

	uint AssignId() { return ++id; }
	void Draw();
public:

	QuadNode* root_node = nullptr;

	std::list<QuadNode*> regions;

	uint id = 0;
};



#endif // !_QUADTREE_H_
