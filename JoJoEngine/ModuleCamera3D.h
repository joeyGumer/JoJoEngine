#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include <list>
#include "p2Point.h"
#include "Math.h"
#include "ComponentCamera.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum Direction
{
	GO_RIGHT,
	GO_LEFT,
	GO_UP,
	GO_DOWN
};

class GameObject;
class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data) const;

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void CenterCameraOnGeometry(const AABB &box);
	void Move(const float3 &Movement);
	void Move(Direction d, float speed);
	GameObject* MousePick();



	void SetPerspective(float aspect_r, float fovy, float n, float f);

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;

	float3 GetPosition() const;

	void Rotate(float x, float y);


	//Transform a 3D point to a point of the screen
	void From3Dto2D(float3 point, int& x, int& y);

private:

	void CalculateViewMatrix();

public:
	
	float3 X, Y, Z, Position, Reference;
	float speed, max_speed;
	float wheel_speed, max_wheel_speed;
	float sensitivity, max_sensitivity;

private:


	mat4x4 ViewMatrix, ViewMatrixInverse;

	ComponentCamera* cam = nullptr;
	LineSegment last_ray;
};