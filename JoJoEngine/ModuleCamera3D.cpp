#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"

#include "JSON\parson.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name = "camera";

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	max_speed = 10.0f;
	max_wheel_speed = 200.0f;
	max_sensitivity = 5.0f;
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

	vec3 newPos(0,0,0);

	//Zoom
	if (App->input->GetMouseZ() > 0) newPos -= Z  * wheel_speed * dt;
	if (App->input->GetMouseZ() < 0) newPos += Z  * wheel_speed * dt;

	//Movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += Y * speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= Y * speed * dt;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed * dt;
	
	Position += newPos;
	Reference = Position - (Z * length(Reference - Position));

	//Camera Focus on Geometry
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_UP)
		CenterCameraOnGeometry(App->renderer3D->GetAABB());

	// Mouse Orbital ----------------
	if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();
		Position -= Reference;
		
		if(dx != 0)
		{
			float DeltaX = (float)dx * sensitivity * dt;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}		
		if(dy != 0)
		{
			float DeltaY = (float)dy * sensitivity * dt;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Y = rotate(Y, -DeltaY, X);
				Z = rotate(Z, -DeltaY, X);
			}
		}
		Position = Reference + Z * length(Position);		
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::CenterCameraOnGeometry(const AABB box)
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

		LookAt(Reference);
	}
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}
// ------------------------------------------------------------------
void ModuleCamera3D::Move(Direction d, float speed)
{
	vec3 newPos(0, 0, 0);
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

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::Rotate(float x, float y)
{
	int dx = -x;
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

	CalculateViewMatrix();
}
// -----------------------------------------------------------------

void ModuleCamera3D::From3Dto2D(vec3 point, int& x, int& y)
{
	mat4x4 projection;
	projection = perspective(60.0f, (float)App->window->GetWidth() / (float)App->window->GetHeight(), 0.125f, 512.0f);

	vec3 screen = multiply(point, ViewMatrix);
	screen = multiply(screen, projection);

	screen.x /= screen.z;
	screen.y /= screen.z;

	x = (screen.x +1) * (App->window->GetWidth() /2);
	y = (screen.y + 1) * (App->window->GetHeight() /2);
}

// -----------------------------------------------------------------
bool ModuleCamera3D::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	JSON_Object* pos = json_object_get_object(data, "pos");
	JSON_Object* reference = json_object_get_object(data, "reference");

	Position.x = json_object_get_number(pos, "x");
	Position.y = json_object_get_number(pos, "y");
	Position.z = json_object_get_number(pos, "z");
	Reference.x = json_object_get_number(reference, "x");
	Reference.y = json_object_get_number(reference, "y");
	Reference.z = json_object_get_number(reference, "z");
	speed = json_object_get_number(data, "speed");
	wheel_speed = json_object_get_number(data, "wheel_speed");
	sensitivity = json_object_get_number(data, "sensitivity");

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
