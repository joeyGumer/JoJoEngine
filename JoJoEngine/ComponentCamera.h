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

	void Update();

	void OnEditor();

	void InitFrustrum();

	void SetAspectRatio(float ratio);
	void SetVerticalFOV(float fov);

	float GetAspectRatio() const;
	float GetVerticalFOV() const;
private:

	float CalculateAspectRatio()const;

private:
	Frustum cam;
};

#endif //_COMPONENTCAMERA_H_

