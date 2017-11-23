#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleGOManager.h"
#include "GameObject.h"

#include "JSON\parson.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <map>

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name = "camera";

	cam = new ComponentCamera(nullptr);

	CalculateViewMatrix();

	Position = float3(5.0f, 5.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	cam->SetCameraFrame(Position, Z, Y);

	speed = max_speed = 5.0f;
	wheel_speed = max_wheel_speed = 200.0f;
	sensitivity = max_sensitivity = 5.0f;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	LookAt(Reference);

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Debug camera mode: Disabled for the final game (but better keep the code)
	float3 movement(0, 0, 0);

	Z = cam->cam.Front();
	Y = cam->cam.Up();
	X = cam->cam.WorldRight();
	Position = cam->cam.Pos();

	//Zoom
	if (App->input->GetMouseZ() > 0) movement -= Z * wheel_speed;
	if (App->input->GetMouseZ() < 0) movement += Z * wheel_speed;

	//Movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += Y * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= Y * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += X * speed;

	if (!movement.IsZero())
	{
		//cam->cam.Translate(movement  * dt);
		cam->cam.SetPos(Position + (movement  * dt));
		Position = cam->cam.Pos();
	}

	Reference = Position - (Z * (Reference - Position).Length());

	/*
	float3 movement(0,0,0);

	//Zoom
	if (App->input->GetMouseZ() > 0) movement -= Z * max_wheel_speed;
	if (App->input->GetMouseZ() < 0) movement += Z * max_wheel_speed;

	//Movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += Y * max_speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= Y * max_speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= X * max_speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += X * max_speed;
	
	if(!movement.IsZero())
		cam->cam.Translate(movement  * dt);*/
	//Reference = Position - (Z * length(Reference - Position));

	//NOTE: sure there's an easier way now
	// Mouse Orbital ----------------
	if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float3 dir = Position - Reference;
		
		Quat qx, qy;

		if(dx != 0)
		{
			float DeltaX = (float)dx * sensitivity * dt;

			qx = qx.RotateY(DeltaX);
			dir = qx.Transform(dir);

		}		
		if(dy != 0)
		{
			float DeltaY = (float)dy * sensitivity * dt;

			qy = qy.RotateAxisAngle(X, DeltaY);
			dir = qy.Transform(dir);

		}
		Position = Reference + dir;
		
		cam->cam.SetPos(Position);

		cam->LookAt(Reference);
	}

	//NOTE: may change the locatoin to another place
	//	Pick GO
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		GameObject* select_go = MousePick();

		if (select_go != nullptr)
			App->go_manager->SetGoSelected(select_go);
	}

	//NOTE: just for testing
	//App->renderer3D->DrawFrustrum(cam->cam);
	App->renderer3D->DrawLineSegment(last_ray);

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	/*this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();*/
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3 &Spot)
{
	cam->LookAt(Spot);
}

// -----------------------------------------------------------------
void ModuleCamera3D::CenterCameraOnGeometry(const AABB &box)
{
	if (box.MaxX() != 0 && box.MaxY() != 0 && box.MaxZ() != 0)
	{
		vec offset(box.maxPoint - box.minPoint);

		Position.x = box.maxPoint.x + offset.Length();
		Position.y = box.maxPoint.y + offset.Length();
		Position.z = box.maxPoint.z + offset.Length();

		Reference.x = box.CenterPoint().x;
		Reference.y = box.CenterPoint().y;
		Reference.z = box.CenterPoint().z;

		cam->cam.SetPos(Position);
		LookAt(Reference);
	}
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	/*Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();*/
}
// ------------------------------------------------------------------
void ModuleCamera3D::Move(Direction d, float speed)
{
	/*vec3 newPos(0, 0, 0);
	switch (d)
	{
	case GO_RIGHT:
		newPos += X * speed;
		break;
	case GO_LEFT:
		newPos -= X * speed;
		break;
	case GO_UP:
		newPos.y += speed;
		break;
	case GO_DOWN:
		newPos.y -= speed;
		break;
	default:
		break;
	}

	Position += newPos;
	Reference += newPos;

	CalculateViewMatrix();*/
}

GameObject* ModuleCamera3D::MousePick() 
{
	float mx = App->input->GetMouseX();
	float my = App->input->GetMouseY();
	float width = App->window->GetWidth();
	float height = App->window->GetHeight();


	mx = (mx * (2 / width)) - 1.0;
	my = 1.0 - (my * (2 / height)) ;

	LineSegment lray = cam->cam.UnProjectLineSegment(mx, my);

	last_ray = lray;
	//last_ray.b = -last_ray.b;

	std::map<float, GameObject*> intersect_go;

	float distance;
	float3 hi_point;
	GameObject* hit_go = App->go_manager->CastRayGO(lray, &hi_point);

	

	/*//NOTE: have to optimize this point
	for (uint i = 0, size = game_objects.size(); i < size; i++)
	{
		GameObject* go = game_objects[i];

		float dist, f;

		//If thay intersects with the AABB, we check for triangles to know the distances from hit point
		if (m_ray.Intersects(go->bb_axis))
		{


		}



	}*/

	return hit_go;
}

//-----------------------------------------------------------------
void ModuleCamera3D::SetPerspective(float aspect_r, float fovy, float n, float f)
{
	cam->SetPerspective(aspect_r, fovy, n, f);
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix() const
{
	return cam->GetViewMatrix();
}

float* ModuleCamera3D::GetProjectionMatrix() const
{
	return cam->GetProjectionMatrix();
}

float3 ModuleCamera3D::GetPosition() const
{
	return cam->cam.Pos();
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	//ViewMatrixInverse = inverse(ViewMatrix);
	//cam.CalculateViewMatrix();
}

void ModuleCamera3D::Rotate(float x, float y)
{
	/*int dx = -x;
	int dy = -y;

	float Sensitivity = 0.25f;

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	Position = Reference + Z * length(Position);

	CalculateViewMatrix();*/
}
// -----------------------------------------------------------------

void ModuleCamera3D::From3Dto2D(float3 point, int& x, int& y)
{
	//NOTE: Why this?

	/*mat4x4 projection;
	projection = perspective(60.0f, (float)App->window->GetWidth() / (float)App->window->GetHeight(), 0.125f, 512.0f);

	vec3 screen = multiply(point, ViewMatrix);
	screen = multiply(screen, projection);

	screen.x /= screen.z;
	screen.y /= screen.z;

	x = (screen.x +1) * (App->window->GetWidth() /2);
	y = (screen.y + 1) * (App->window->GetHeight() /2);*/
}

// -----------------------------------------------------------------
bool ModuleCamera3D::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	JSON_Object* pos = json_object_get_object(data, "pos");
	JSON_Object* reference = json_object_get_object(data, "reference");

	/*Position.x = json_object_get_number(pos, "x");
	Position.y = json_object_get_number(pos, "y");
	Position.z = json_object_get_number(pos, "z");
	Reference.x = json_object_get_number(reference, "x");
	Reference.y = json_object_get_number(reference, "y");
	Reference.z = json_object_get_number(reference, "z");
	speed = json_object_get_number(data, "speed");
	wheel_speed = json_object_get_number(data, "wheel_speed");
	sensitivity = json_object_get_number(data, "sensitivity");*/

	return ret;
}

bool ModuleCamera3D::SaveConfig(JSON_Object* data) const
{
	bool ret = true;

	JSON_Object* pos = json_object_get_object(data, "pos");
	JSON_Object* reference = json_object_get_object(data, "reference");

	json_object_set_number(pos, "x", Position.x);
	json_object_set_number(pos, "y", Position.y);
	json_object_set_number(pos, "z", Position.z);
	json_object_set_number(reference, "x", Reference.x);
	json_object_set_number(reference, "y", Reference.y);
	json_object_set_number(reference, "z", Reference.z);
	json_object_set_number(data, "speed",speed);
	json_object_set_number(data, "wheel_speed", wheel_speed);
	json_object_set_number(data, "sensitivity", sensitivity);

	return ret;
}
