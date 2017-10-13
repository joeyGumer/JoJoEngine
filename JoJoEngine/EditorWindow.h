#ifndef __EDITORWINDOW_H__
#define __EDITORWINDOW_H__


class EditorWindow
{
public:
	EditorWindow(){}
	virtual ~EditorWindow(){}

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void CleanUp() = 0;

public:
	bool is_open = false;
	bool want_to_be_open = false;

};
#endif __EDITORWINDOW_H__

