#ifndef __EDITORWINDOW_H__
#define __EDITORWINDOW_H__


class EditorWindow
{
public:
	EditorWindow(){}
	virtual ~EditorWindow(){}


	virtual void Update() = 0;

public:
	bool is_open = false;

};
#endif __EDITORWINDOW_H__

