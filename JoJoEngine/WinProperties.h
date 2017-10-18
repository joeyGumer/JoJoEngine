#ifndef _WINPROPERTIES_H_
#define _WINPROPERTIES_H_

#include "EditorWindow.h"

class GameObject;
class WinProperties : public EditorWindow
{
public:
	WinProperties();
	~WinProperties();

	void Start();
	void Update();
	void CleanUp();

	//NOTE: really needed to have go in private?
	void SetGameObject(GameObject* g);

private:
	GameObject* go = nullptr;
};
#endif // !_WINPROPERTIES_H_

