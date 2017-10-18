#include "WinProperties.h"

#include "Application.h"
#include "GameObject.h"
#include "Component.h"

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

void WinProperties::CleanUp()
{

}

void WinProperties::Update()
{
	if (is_open)
	{
		ImGui::Begin("Properties", &is_open);

		if (go)
		{
			for (uint i = 0, size = go->components.size(); i < size; i++)
			{
				go->components[i]->OnEditor();
			}
		}
		/*uint size = App->renderer3D->meshes_array.size();
		if(ImGui::CollapsingHeader("Transform"))
		{
			if (size > 0)
			{
				float4x4 tmp_mat = App->renderer3D->transform;

				float3 tmp_pos;
				float4x4 tmp_rot_mat;
				float3 tmp_scale;

				tmp_mat.Decompose(tmp_pos, tmp_rot_mat, tmp_scale);

				float3 tmp_rot = tmp_rot_mat.ToEulerXYZ();

				ImGui::Text("Position :");
				ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", tmp_pos.x, tmp_pos.y, tmp_pos.z);
				ImGui::Text("Rotation :");
				ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", tmp_rot.x, tmp_pos.y, tmp_pos.z);
				ImGui::Text("Scale :");
				ImGui::Text("X: %.3f  Y: %.3f  Z: %.3f", tmp_scale.x, tmp_scale.y, tmp_scale.z);
			}

			else
				ImGui::Text("No FBX loaded");
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

				std::string mesh_name = std::string("Mesh ") + std::to_string(i + 1);

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
			if (App->renderer3D->GetTextureChannel() != 0)
			{
				ImGui::Image((ImTextureID)App->renderer3D->GetTextureChannel(), ImVec2(200, 200));
				ImGui::Text("%i x %i", (int)App->renderer3D->GetTextureSize().x, (int)App->renderer3D->GetTextureSize().y);
			}
			else
				ImGui::Text("No texture loaded");
		}*/

		ImGui::End();
	}
}

void WinProperties::SetGameObject(GameObject* g)
{
	go = g;
}