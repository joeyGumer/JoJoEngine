#ifndef __IMGUIDEMO_H__
#define __IMGUIDEMO_H__

#include "EditorWindow.h"

class ImGuiDemo : public EditorWindow
{
public:
	ImGuiDemo();
	~ImGuiDemo();

	void Start();
	void Update();
	void CleanUp();

private:
};

#endif
