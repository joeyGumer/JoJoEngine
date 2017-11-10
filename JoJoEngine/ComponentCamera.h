#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Component.h"
#include "Math.h"

class GameObject;

class ComponentCamera: public Component
{

public:
	ComponentCamera(GameObject* go);
	~ComponentCamera();

	void SetFrustrum();

private:
	Frustum cam;
};

#endif //_COMPONENTCAMERA_H_

