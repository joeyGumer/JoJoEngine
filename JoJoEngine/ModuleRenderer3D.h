#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

struct Model3D;
class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data);

	void OnResize(int width, int height, float fovy);

	void Draw(Model3D* mesh);
	void DrawMeshes();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	std::vector<Model3D*> meshes_array;
	uint num_meshes;
};