#ifndef _WINHIERARCHY_H_
#define _WINHIERARCHY_H_

#include "EditorWindow.h"
#include "Globals.h"

class GameObject;
class WinHierarchy : public EditorWindow
{
public:
	WinHierarchy();
	~WinHierarchy();

	void Start();
	void Update();
	void CleanUp();

private:

	void ShowGO(GameObject* go);

private:
	uint go_id = 0;
	char label[100];
};

#endif