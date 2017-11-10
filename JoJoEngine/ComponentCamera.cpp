#include "ComponentCamera.h"


ComponentCamera::ComponentCamera(GameObject* go): Component(COMP_CAMERA, go)
{
	SetFrustrum();
}
ComponentCamera::~ComponentCamera()
{

}


void ComponentCamera::SetFrustrum()
{
	//NOTE: testing
	cam.SetFrame(float3::zero, { 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0 });
	cam.SetViewPlaneDistances(1.0, 10);
	cam.SetPerspective(90, 90);
}