#include "WinConsole.h"

WinConsole::WinConsole()
{
}

WinConsole::~WinConsole()
{	
}

void WinConsole::Start()
{
}

void WinConsole::CleanUp()
{
	buffer.clear();
}

void WinConsole::Update()
{
	if (is_open)
	{
		ImGui::Begin("Console", &is_open);

		ImGui::TextUnformatted(buffer.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;

		ImGui::End();
	}
}

void WinConsole::Write(const char* text)
{
	buffer.append(text);
	ScrollToBottom = true;
}