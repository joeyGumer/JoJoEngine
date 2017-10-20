#include "ComponentMaterial.h"
#include "Imgui/imgui.h"

ComponentMaterial::ComponentMaterial(GameObject* g) : Component(COMP_MATERIAL, g)
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

void ComponentMaterial::SetTexture(uint tex, float x, float y)
{
	texture = tex;
	size = { x, y };
}

uint ComponentMaterial::GetTexture() const
{
	return texture;
}

