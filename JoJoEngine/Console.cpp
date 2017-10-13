#include "Console.h"

Console::Console()
{
}

Console::~Console()
{	
}

void Console::Start()
{
}

void Console::CleanUp()
{
	buffer.clear();
}

void Console::Update()
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

void Console::Write(const char* text)
{
	buffer.append(text);
	ScrollToBottom = true;
}