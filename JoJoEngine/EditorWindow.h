#ifndef _EDITORWINDOW_H_
#define _EDITORWINDOW_H_

class EditorWindow
{
public:
	EditorWindow(){}
	virtual ~EditorWindow(){}

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void CleanUp() = 0;

	void OpenCloseWindow();

public:
	bool is_open = false;
	bool want_to_be_open = false;
};

#endif // !_EDITORWINDOW_H_
