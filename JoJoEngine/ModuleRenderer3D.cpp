#include "Globals.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneEdit.h"
#include "ModuleEditor.h"
#include "ModuleFBXLoader.h"
#include "ComponentCamera.h"

#include "JSON\parson.h"
#include "OpenGl.h"

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"

//Devil NOTE: may not be better to put it here
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")


ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module( start_enabled)
{
	name = "renderer";

	num_meshes = 0;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Initialize Devil NOTE: may have to be ported
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	//Create OpenGL context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum gl_enum = glewInit();

	if (GLEW_OK != gl_enum)
	{
		LOG("Glew failed");
	}
	else
	{
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(App->window->GetVsync() && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Detecting Hardware and driver compatibilities
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGl version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		//OpenGl Initialization 
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);


		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		//glEnable(GL_LIGHTING);
		//glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(App->window->GetWidth(), App->window->GetHeight(), 20.0f);

	ImGui_ImplSdlGL3_Init(App->window->window);

	return ret;
}

//NOTE: look how to organize the order of loading
bool ModuleRenderer3D::Start()
{
	bool ret = true;

	SetEnableColorMaterial(enable_color_material);
	SetEnableCullFace(enable_face_culling);
	SetEnableDepth(enable_depth);
	SetEnableLight(enable_lighting);
	SetEnableTextures(enable_textures);

	return true;
}
// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	ImGui_ImplSdlGL3_Shutdown();

	//Delete all meshes
	UnloadScene();

	ilShutDown();

	//Delete OpenGl context
	SDL_GL_DeleteContext(context);

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	//Color c = App->camera->background;
	//glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	float3 pos = App->camera->GetPosition();

	lights[0].SetPos(pos.x, pos.y, pos.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//Draw 
	App->level->Draw();
	//DrawMeshes();
	/*if (debug_draw == true)
	{
		BeginDebugDraw();
		App->DebugDraw();
		EndDebugDraw();
	}*/
	App->editor->Draw();
	
	//Swap window for next frame
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}


bool ModuleRenderer3D::LoadImageTexture(const char* file)
{

	LOG("Loading new Texture into scene---------------");

	bool ret = true;
	ILuint id_image;
	ilGenImages(1, &id_image);
	ilBindImage(id_image);

	ret = ilLoadImage(file);

	if (ret)
	{
		//NOTE: maybe check if the image is a power of two?

		texture_channel = ilutGLBindTexImage();

		texture_size.x = ilGetInteger(IL_IMAGE_WIDTH);
		texture_size.y = ilGetInteger(IL_IMAGE_HEIGHT);

		/*texture_channel = 0;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texture_channel);
		glBindTexture(GL_TEXTURE_2D, texture_channel);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 
			0, 
			ilGetInteger(IL_IMAGE_FORMAT), 
			ilGetInteger(IL_IMAGE_WIDTH),   // Image width
			ilGetInteger(IL_IMAGE_HEIGHT), 
			0, 
			ilGetInteger(IL_IMAGE_FORMAT), //NOTE: may have to change this
			GL_UNSIGNED_BYTE, 
			ilGetData());

		glBindTexture(GL_TEXTURE_2D, 0);*/
	}
	else
	{
		LOG("Error: failure trying to load texture %s", file);
	}

	ilDeleteImages(1, &id_image);

	return ret;
}

//NODE: to delete
void ModuleRenderer3D::UnloadScene()
{
	LOG("Unloading all meshes from the scene");

	num_meshes = 0;
}

//-----------------------------------------------
// DRAW FUNCTIONS
//-----------------------------------------------

//NOTE: pass as references? or pointer? or copy?
void ModuleRenderer3D::Draw(const Mesh* mesh, float4x4 &transform, uint &texture_id) const
{
	glPushMatrix();
	glMultMatrixf(*transform.v);

	glColor3f(1.0f, 1.0f, 1.0f);

	//Draw 	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glNormalPointer(GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_UVs);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	if (draw_meshes)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
	}

	//Wireframe
	DrawWireframe(mesh);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

}

void ModuleRenderer3D::DrawNormals(const Mesh* mesh) const
{
	if (draw_normals)
	{
		//NOTE: trying to draw them with direct mode, temporaly
		glBegin(GL_LINES);

		glLineWidth(1.0f);
		glColor3f(0.0f, 1.0, 1.0);

		for (int i = 0; i < mesh->num_normals * 3; i += 3)
		{
			float3 vertex = { mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2] };
			float3 n_vertex = vertex + float3(mesh->normals[i], mesh->normals[i + 1], mesh->normals[i + 2]);

			glVertex3f(vertex.x, vertex.y, vertex.z);
			glVertex3f(n_vertex.x, n_vertex.y, n_vertex.z);
		}

		glEnd();
	}
}

void ModuleRenderer3D::DrawWireframe(const Mesh* mesh) const
{
	if (draw_wireframe)
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		glLineWidth(2.0f);

		if(enable_textures)
			glDisable(GL_TEXTURE_2D);

		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);

		if (enable_textures)
			glEnable(GL_TEXTURE_2D);

		glEnable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::DrawOBB(const OBB* box)const
{
	float3 box_vertices[24];

	box->ToEdgeList(box_vertices);

	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glColor3f(3.0f, 0.0f, 0.0f);


	for (uint i = 0; i < 12; i++)
	{
		uint tmp = i * 2;

		float3 v = box_vertices[tmp];
		glVertex3d(v.x, v.y, v.z);
		v = box_vertices[tmp + 1];
		glVertex3d(v.x, v.y, v.z);
	}

	glEnd();
}

void ModuleRenderer3D::DrawFrustrum(const Frustum& frustum) const
{

	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 3.0f);

	for (uint i = 0; i < 12; i++)
	{
		LineSegment edge = frustum.Edge(i);

		float3 v = edge.a;
		glVertex3d(v.x, v.y, v.z);
		v = edge.b;
		glVertex3d(v.x, v.y, v.z);
	}

	glEnd();
}

void ModuleRenderer3D::DrawAABB(const AABB* box) const
{
	
	

	//NOTE: And using box function?
	float3 box_vertices[8];

	float3 min = box->minPoint;
	float3 max = box->maxPoint;

	box_vertices[0] = min;
	box_vertices[1] = { max.x, min.y, min.z };
	box_vertices[2] = { max.x, max.y, min.z };
	box_vertices[3] = { min.x, max.y, min.z };
	box_vertices[4] = { min.x, min.y, max.z };
	box_vertices[5] = { max.x, min.y, max.z };
	box_vertices[6] = max;
	box_vertices[7] = { min.x, max.y, max.z };

	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glColor3f(0.0f, 3.0f, 0.0f);

	float3 v;

	//NOTE: just thingds done because too lazy to program
	for (int i = 0; i <= 4; i += 4)
	{
		v = box_vertices[i];
		glVertex3d(v.x, v.y, v.z);
		v = box_vertices[i+1];
		glVertex3d(v.x, v.y, v.z);

		glVertex3d(v.x, v.y, v.z);
		v = box_vertices[i+2];
		glVertex3d(v.x, v.y, v.z);

		glVertex3d(v.x, v.y, v.z);
		v = box_vertices[i+3];
		glVertex3d(v.x, v.y, v.z);

		glVertex3d(v.x, v.y, v.z);
		v = box_vertices[i];
		glVertex3d(v.x, v.y, v.z);
	}

	for (int i = 0, j = i + 4; i < 4; i++, j++)
	{
		v = box_vertices[i];
		glVertex3d(v.x, v.y, v.z);
		v = box_vertices[j];
		glVertex3d(v.x, v.y, v.z);
	}

	glEnd();

};



void ModuleRenderer3D::OnResize(int width, int height, float fovy)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//NOTE: this may give some errors
	//App->camera->SetPerspective((float)width / (float)height, fovy, 0.125f, 512.0f);
	ProjectionMatrix = ProjectionMatrix.perspective(fovy, (float)width / (float)height, 0.125f, 512.0f);//App->camera->GetProjectionMatrix();
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Utiles
//Getters
bool ModuleRenderer3D::GetEnableTextures() const
{
	return enable_textures;
}
bool ModuleRenderer3D::GetEnableColorMaterial() const
{
	return enable_color_material;
}
bool ModuleRenderer3D::GetEnableDepth() const
{
	return enable_depth;
}
bool ModuleRenderer3D::GetEnableCullFace() const
{
	return enable_face_culling;
}
bool ModuleRenderer3D::GetEnableLight() const
{
	return enable_lighting;
}

//Setters
void ModuleRenderer3D::SetEnableTextures(const bool& enable)
{
	enable_textures = enable;

	if (enable_textures)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SetEnableColorMaterial(const bool& enable)
{
	enable_color_material = enable;

	if (enable_color_material)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::SetEnableDepth(const bool& enable)
{
	enable_depth = enable;

	if (enable_depth)
		glEnable(GL_DEPTH);
	else
		glDisable(GL_DEPTH);
}

void ModuleRenderer3D::SetEnableCullFace(const bool& enable)
{
	enable_face_culling = enable;

	if (enable_face_culling)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::SetEnableLight(const bool& enable)
{
	enable_lighting = enable;

	if (enable_lighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

bool ModuleRenderer3D::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	draw_normals = json_object_get_boolean(data, "draw_normals");
	draw_wireframe = json_object_get_boolean(data, "draw_wireframe");
	draw_meshes = json_object_get_boolean(data, "draw_meshes");
	enable_depth = json_object_get_boolean(data, "depth");
	enable_face_culling = json_object_get_boolean(data, "face_culling");
	enable_lighting = json_object_get_boolean(data, "lighting");
	enable_color_material = json_object_get_boolean(data, "color_material");
	enable_textures = json_object_get_boolean(data, "draw_textures");

	return ret;
}

bool ModuleRenderer3D::SaveConfig(JSON_Object* data) const
{
	bool ret = true;

	json_object_set_boolean(data, "draw_normals", draw_normals);
	json_object_set_boolean(data, "draw_wireframe", draw_wireframe);
	json_object_set_boolean(data, "draw_meshes", draw_meshes);
	json_object_set_boolean(data, "depth", enable_depth);
	json_object_set_boolean(data, "face_culling", enable_face_culling);
	json_object_set_boolean(data, "lighting", enable_lighting);
	json_object_set_boolean(data, "color_material", enable_color_material);
	json_object_set_boolean(data, "draw_textures", enable_textures);

	return ret;
}

const uint ModuleRenderer3D::GetTextureChannel() const
{
	return texture_channel;
}

const vec ModuleRenderer3D::GetTextureSize() const
{
	return texture_size;
}

//NOTE: transport to individual mesh
const AABB ModuleRenderer3D::GetAABB() const
{
	AABB ret(float3(0, 0, 0), float3(0, 0, 0));

	/*if (!meshes_array.empty())
	{
		std::vector<float3> vertices;
		//Calculate a AABB for each individual mesh
		for (int i = 0; i < meshes_array.size(); i++)
		{
			AABB individual_aabb(float3(0, 0, 0), float3(0, 0, 0));
			std::vector <float3> vertex_array;

			for (int j = 0; j < meshes_array[i]->num_vertices * 3; j += 3)
				vertex_array.push_back(float3(meshes_array[i]->vertices[j], meshes_array[i]->vertices[j + 1], meshes_array[i]->vertices[j + 2]));

			individual_aabb.Enclose(&vertex_array[0], meshes_array[i]->num_vertices);

			//Stores each vertex of each individual AABB on an array to generate the global AABB later
			for (int k = 0; k < 8; k++)
				vertices.push_back(individual_aabb.CornerPoint(k));
		}
		//Global AABB
		ret.Enclose(&vertices[0], vertices.size());
	}*/
	return ret;
}