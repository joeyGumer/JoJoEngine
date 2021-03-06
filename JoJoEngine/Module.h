#pragma once

#include "Globals.h"
#include <string>

typedef struct json_object_t JSON_Object;
struct PhysBody3D;


class Module
{
private :
	bool enabled;

public:

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool LoadConfig(JSON_Object* data)
	{
		return true;
	}

	virtual bool SaveConfig(JSON_Object* data) const
	{
		return true;
	}

	virtual bool Init()
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	std::string name; 
};