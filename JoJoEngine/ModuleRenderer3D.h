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
	bool SaveConfig(JSON_Object* data)const;

	void DrawMeshes() const;

	void DrawNormals(const Mesh* mesh) const;
	void DrawWireframe(const Mesh* mesh) const;


public:
	void DrawAABB(const AABB* box)const;
	void DrawOBB(const OBB* box)const;
	void DrawFrustrum(const Frustum& frustum)const;

	void Draw(const Mesh* mesh, float4x4 &transform, uint &texture_id) const;

	void OnResize(int width, int height, float fovy);

	bool LoadImageTexture(const char* file);
	void UnloadScene();

	//Getters
	bool GetEnableTextures() const;
	bool GetEnableColorMaterial() const;
	bool GetEnableDepth() const;
	bool GetEnableCullFace() const;
	bool GetEnableLight() const;

	//Setters
	void SetEnableTextures(const bool& enable);
	void SetEnableColorMaterial(const bool& enable);
	void SetEnableDepth(const bool& enable);
	void SetEnableCullFace(const bool& enable);
	void SetEnableLight(const bool& enable);


	const AABB GetAABB() const;
	const uint GetTextureChannel() const;
	const vec GetTextureSize() const;



public:
	//Geometry
	bool draw_normals;
	bool draw_wireframe;
	bool draw_meshes;


	//-------------------
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float* ModelMatrix;
	float* ViewMatrix;
	mat4x4 ProjectionMatrix;

	//NOTE: transform matrix, temporal, until we use GO
	float4x4 transform;

	//std::vector<Mesh*> meshes_array;
	uint num_meshes = 0;	

private:
	uint texture_channel = 0;
	vec texture_size;

	//Geometry
	bool enable_depth;
	bool enable_face_culling;

	//Lights
	bool enable_lighting;

	//Textures
	bool enable_color_material;
	bool enable_textures;
};