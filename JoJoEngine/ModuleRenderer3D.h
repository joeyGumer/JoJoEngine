#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

struct Mesh;
class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

private:
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data);

	void Draw(const Mesh* mesh) const;
	void DrawMeshes() const;

	void DrawNormals(const Mesh* mesh) const;
	void DrawWireframe(const Mesh* mesh) const;

public:

	void OnResize(int width, int height, float fovy);

	bool LoadMesh(char* file);
	bool LoadImageTexture(char* file);
	void EnableTextures(bool enable);

	const AABB GetAABB() const;
	const vec GetTextureSize() const;

public:
	bool draw_normals = false;
	bool draw_wireframe = false;
	bool draw_textures = true;
	bool draw_meshes = true;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	std::vector<Mesh*> meshes_array;
	uint num_meshes = 0;
	int current_texture_id = 0;
	vec texture_size;

private:
	uint texture_channel = 0;
};