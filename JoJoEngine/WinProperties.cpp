#include "WinProperties.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleFBXLoader.h"

#include "Imgui/imgui.h"

WinProperties::WinProperties()
{

}
WinProperties::~WinProperties()
{

}

void WinProperties::Start()
{

}

void WinProperties::Update()
{
	if (is_open)
	{
		ImGui::Begin("Properties", &is_open);

		uint size = App->renderer3D->meshes_array.size();
		if(ImGui::CollapsingHeader("Transform"))
		{ 
		
		}
		if (ImGui::CollapsingHeader("Geometry"))
		{
			uint total_vertices = 0;
			uint total_faces = 0;

			for (uint i = 0; i < size; i++)
			{
				Mesh* tmp_mesh = App->renderer3D->meshes_array[i];

				uint n_vertices = tmp_mesh->num_vertices;
				uint n_indices = tmp_mesh->num_indices / 3;

				string mesh_name = string("Mesh ") + to_string(i + 1);

				if (ImGui::TreeNodeEx(mesh_name.c_str()))
				{
					ImGui::Text("Num Vertices:  %d", n_vertices);
					ImGui::Text("Num Triangles: %d", n_indices);

					ImGui::TreePop();
				}

				total_vertices += n_vertices;
				total_faces += n_indices;
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text("Total vertices: %d", total_vertices);
			ImGui::Text("Total triangles: %d", total_faces);
		}
		if (ImGui::CollapsingHeader("Texture"))
		{
			ImGui::Image((ImTextureID)App->renderer3D->GetTextureChannel(), ImVec2(200, 200));
			ImGui::Text("%i x %i", (int)App->renderer3D->GetTextureSize().x, (int)App->renderer3D->GetTextureSize().y);
		}

		ImGui::End();
	}
}

void WinProperties::CleanUp()
{

}