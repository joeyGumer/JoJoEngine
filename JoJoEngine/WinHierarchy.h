#ifndef _WINHIERARCHY_H_
#define _WINHIERARCHY_H_

#include "EditorWindow.h"

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


};

#endif