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
	void SetAsMainCamera(bool is_main_cam);

	float GetAspectRatio() const;
	float GetVerticalFOV() const;
	bool IsMainCamera() const;


	bool CullGameObject(GameObject* go) const;

public:
	bool frustum_culling;
private:
	Frustum cam;
	bool main_cam;
};

#endif //_COMPONENTCAMERA_H_

