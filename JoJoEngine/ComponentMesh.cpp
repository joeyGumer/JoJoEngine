#include "ComponentMesh.h"

//NOTE: should separate mesh definition?
#include "ModuleFBXLoader.h"
#include "Globals.h"
#include "Imgui/imgui.h"

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
	if (ImGui::CollapsingHeader("Mesh"))
	{
		uint n_vertices = mesh->num_vertices;
		uint n_triangles = mesh->num_indices / 3;
		ImGui::Text("Num Vertices:  %d", n_vertices);
		ImGui::Text("Num Triangles: %d", n_triangles);
	}
}