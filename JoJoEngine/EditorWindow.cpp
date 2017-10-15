#include "EditorWindow.h"
#include "Application.h"
#include "ModuleEditor.h"

void EditorWindow::OpenCloseWindow()
{
	if (App->editor->IsActive())
	{
		is_open = !is_open;
	}
}
