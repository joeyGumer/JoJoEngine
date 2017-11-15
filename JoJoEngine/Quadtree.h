#pragma once
#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Math.h"
#include "Globals.h"
#include <vector>

class GameObject;

struct QuadNode
{

	AABB limits;

	QuadNode* parent = nullptr;
	std::vector<QuadNode*> children;

	std::vector<GameObject*> game_objects;
	uint go_limit = 4;
};

class Quadtree
{
public:
	void Create(AABB& limits);
	void Clear();
	void Insert(GameObject*);
	void Remove(GameObject*);
	//void Intersect(std::vector<GameObject*>&, PRIMITIVE);

	void Draw();
private:
	QuadNode root_node;

};


#endif // !_QUADTREE_H_
