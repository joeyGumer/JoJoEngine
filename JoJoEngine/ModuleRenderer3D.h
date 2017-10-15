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
	bool LoadImageTexture(const char* file);
	void UnloadScene();

	void EnableTextures(const bool& enable);
	void EnableColorMaterial(const bool& enable);
	void EnableDepth(const bool& enable);
	void EnableCullFace(const bool& enable);
	void EnableLight(const bool& enable);

	const AABB GetAABB() const;
	const uint GetTextureChannel() const;
	const vec GetTextureSize() const;

public:
	//Geometry
	bool draw_normals;
	bool draw_wireframe;
	bool draw_meshes;
	bool depth;
	bool face_culling;

	//Lights
	bool lighting;

	//Textures
	bool color_material;
	bool draw_textures;

	//-------------------
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	//NOTE: transform matrix, temporal, until we use GO
	float4x4 transform;

	std::vector<Mesh*> meshes_array;
	uint num_meshes = 0;	

private:
	uint texture_channel = 0;
	vec texture_size;
};