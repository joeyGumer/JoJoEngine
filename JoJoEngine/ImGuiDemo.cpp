#include "ImGuiDemo.h"
#include "Imgui/imgui.h"


ImGuiDemo::ImGuiDemo()
{
}

ImGuiDemo::~ImGuiDemo()
{
}

void ImGuiDemo::Start()
{
}

void ImGuiDemo::Update()
{
	if(is_open)
		ImGui::ShowTestWindow(&is_open);		
}

void ImGuiDemo::CleanUp()
{
}
