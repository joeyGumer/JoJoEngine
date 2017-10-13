#ifndef _WINPROPERTIES_H_
#define _WINPROPERTIES_H_

#include "EditorWindow.h"

class WinProperties : public EditorWindow
{
public:
	WinProperties();
	~WinProperties();

	void Start();
	void Update();
	void CleanUp();
};
#endif // !_WINPROPERTIES_H_

