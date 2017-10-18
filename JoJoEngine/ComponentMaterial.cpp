#include "ComponentMaterial.h"
#include "Imgui/imgui.h"

ComponentMaterial::ComponentMaterial() : Component(COMP_MATERIAL)
{

}
ComponentMaterial::ComponentMaterial(uint tex, float2 &s) : Component(COMP_MATERIAL), texture(tex), size(s)
{
	
}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::Update()
{

}

void ComponentMaterial::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		if (texture != 0)
		{
			ImGui::Image((ImTextureID)texture, ImVec2(200, 200));
			ImGui::Text("%i x %i", (int)size.x, (int)size.y);
		}
		else
			ImGui::Text("No texture loaded");
	}
}

