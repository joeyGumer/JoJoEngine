#include "ComponentMesh.h"

//NOTE: should separate mesh definition?
#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"
#include "Globals.h"
#include "Imgui/imgui.h"

#include "OpenGl.h"

ComponentMesh::ComponentMesh(GameObject* g): Component(COMP_MESH, g)
{

}

ComponentMesh::~ComponentMesh()
{
	glDeleteBuffers(1, (GLuint*) &(mesh->id_vertices));
	glDeleteBuffers(1, (GLuint*) &(mesh->id_normals));
	glDeleteBuffers(1, (GLuint*) &(mesh->id_texture_UVs));
	glDeleteBuffers(1, (GLuint*) &(mesh->id_indices));

	RELEASE(mesh);
}

void ComponentMesh::Update()
{
	App->renderer3D->Draw(mesh);
}

void ComponentMesh::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		//Active
		bool bool_tmp = IsActive();
		if (ImGui::Checkbox("Active", &bool_tmp))
		{
			SetActive(bool_tmp);
		}

		//
		uint n_vertices = mesh->num_vertices;
		uint n_triangles = mesh->num_indices / 3;
		ImGui::Text("Num Vertices:  %d", n_vertices);
		ImGui::Text("Num Triangles: %d", n_triangles);
	}
}

void ComponentMesh::SetMesh(Mesh* m)
{
	//NOTE:: Load buffers here?
	
	mesh = m;

	if (mesh)
	{
		glGenBuffers(1, (GLuint*) &(mesh->id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh->id_texture_UVs));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_UVs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* mesh->num_texture_UVs * 2, mesh->texture_UVs, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh->id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh->id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
	}
}