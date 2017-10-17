#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(): Component(COMP_MESH)
{

}
ComponentMesh::ComponentMesh(Mesh* m): Component(COMP_MESH), mesh(m)
{

}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::Update()
{

}

void ComponentMesh::OnEditor()
{

}