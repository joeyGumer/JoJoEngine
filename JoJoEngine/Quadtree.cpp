#include "Quadtree.h"

//NOTE: not sure if it's needed to add the render, or put it in GO_manager
#include "Application.h"
#include "ModuleRenderer3D.h"

void Quadtree::Create(AABB& limits)
{
	root_node.limits = limits;
}

void Quadtree::Clear()
{

}
void Quadtree::Insert(GameObject*)
{

}
void Quadtree::Remove(GameObject*)
{

}
//void Quadtree::Intersect(std::vector<GameObject*>&, PRIMITIVE);

void Quadtree::Draw()
{
	App->renderer3D->DrawAABB(&root_node.limits);
}