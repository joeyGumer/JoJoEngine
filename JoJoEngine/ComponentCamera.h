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
	void OnTransform();

	void InitFrustrum();

	void SetAspectRatio(float ratio);
	void SetVerticalFOV(float fov);
	void SetAsMainCamera(bool is_main_cam);
	void SetCameraFrame(float3& pos, float3& front, float3& up);
	void SetPerspective(float aspect_ratio, float fovy, float n, float f);

	float GetAspectRatio() const;
	float GetVerticalFOV() const;

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;

	void LookAt(const float3& ref);

	bool IsMainCamera() const;


	bool CullGameObject(GameObject* go) const;

public:
	bool frustum_culling;
	Frustum cam;
private:
	bool main_cam;
};

#endif //_COMPONENTCAMERA_H_

