#ifndef _COMPONENT_H_
#define _COMPONENT_H_


enum TypeComp
{
	COMP_TRANSFORM,
	COMP_MESH,
	COMP_MATERIAL,
};

class Component
{
public:
	Component(TypeComp t) : type(t) {};
	virtual ~Component() {};

	virtual void Enable() {}
	virtual void Update() = 0;
	virtual void Disable(){}

	virtual void OnEditor(){}

private:
	TypeComp type;
};
#endif
