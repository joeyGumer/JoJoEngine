#include "WinProperties.h"

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



		ImGui::End();
	}
}

void WinProperties::CleanUp()
{

}